open Operators;
open Rx.Operators;
open Utils.Rx;

/**
 * designated for displaying user snackbar notifications 
 * captured from dispatched actions
 */

/** 
  ASSUMED TO BE ATTACHED ONLY ONCE
  as we hack with stealing the reducer dispatch and setting it to a global context
 */
module Context = {
  type action =
    | SetWarningMessage(option(string))
    | SetErrorMessage(option(string))
    | SetSuccessMessage(option(string));

  type state = {
    message: option((string, Snackbar.notificationType)),
  };

  /** 
    dispatch reference exposed globally (kinda hacky)
   */
  let dispatch = ref(None);

  let snackbarReducer = (_state: state, action) => 
    switch(action){
    | SetWarningMessage(warningMessage) => { 
      message: warningMessage |. Belt.Option.map(message => (message, Snackbar.Warning)) 
    }
    | SetErrorMessage(errorMessage) => {
      message: errorMessage |. Belt.Option.map(message => (message, Snackbar.Error))
    }
    | SetSuccessMessage(successMessage) => {
      message: successMessage |. Belt.Option.map(message => (message, Snackbar.Success))
    }
  };

  [@react.component]
  let make = () => {
    let (state, send) = React.useReducer(snackbarReducer, { message: None });
    /** hack: expose reducer dispatch globally */
    React.useEffect0(() => {
      dispatch := Some(send);
      Some(() => { dispatch := None })
    })

    let message = state.message |. Belt.Option.map(((message, _messageType)) => message);
    let type_ = state.message |. Belt.Option.map(((_message, messageType)) => messageType);

    <Snackbar 
      ?type_
      key=(message |? "message") 
      isOpen=(!?state.message)
      ?message
      onExited=(_event => {
        send(SetSuccessMessage(None));
      })/>
  };
}

let cognitoErrors = (reduxObservable: Rx.Observable.t(('action, 'state))) => {
  reduxObservable
  |> optMap(fun 
    | (`SignInError(error, _), _)
    | (`CompleteNewPasswordRequestError(error), _)
    | (`SignUpError(error), _)
    | (`SignUpRequestRejected(error), _)
    | (`SignOutError(error), _)
    | (`ResendVerificationError(error), _) => Some((error, Context.dispatch^)) 
    /* don't show notifications for code missmatch and code expired as they are handled in Auth */
    | (`ConfirmSignUpError(error, _, _), _) 
      when 
         (error |. AWSAmplify.Error.codeGet != "CodeMismatchException")
      && (error |. AWSAmplify.Error.codeGet != "ExpiredCodeException") => Some((error, Context.dispatch^)) 
    | _ => None)
  |> optMap(fun | (error, Some(dispatch)) => Some((error, dispatch)) | _ => None)
  |> tap(~next=((error: AWSAmplify.Error.t, dispatch)) => 
    dispatch(
      error|.AWSAmplify.Error.codeGet == "CRITICAL" 
        ? Context.SetErrorMessage(Some(error|.AWSAmplify.Error.messageGet))
        : Context.SetWarningMessage(Some(error|.AWSAmplify.Error.messageGet))
    ))
  |> take(0);
};

let cognitoSuccesses = (reduxObservable: Rx.Observable.t(('action, 'state))) => {
  reduxObservable
  |> optMap(fun
    | (`ResendVerificationCompleted(_), _) => Some(("Verification code has been send. Please check your mailbox.", Context.dispatch^))
    | (`ConfirmSignUpCompleted(_), _) => Some(("Account verification completed. Continue by signing in.", Context.dispatch^))
    | (`SignInCompleted(_), _) => Some(("Sign in completed.", Context.dispatch^))
    | _ => None)
  |> optMap(fun | (error, Some(dispatch)) => Some((error, dispatch)) | _ => None)
  |> tap(~next=((message, dispatch)) => 
    dispatch(Context.SetSuccessMessage(Some(message))))
  |> take(0);
};

let epic = reduxObservable => 
  Rx.merge([|
    reduxObservable |> cognitoSuccesses,
    reduxObservable |> cognitoErrors |> Obj.magic,
  |]);
