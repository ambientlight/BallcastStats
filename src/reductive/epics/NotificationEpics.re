open Operators;
open Rx.Observable.Operators;
open ReductiveObservable.Utils;
open Utils.Rx;

/**
 * designated for displaying user snackbar notifications 
 * captured from dispatched actions
 */

/* ASSUMED TO BE ATTACHED ONCE */
module Context {
  type action =
    | SetWarningMessage(option(string))
    | SetErrorMessage(option(string))
    | SetSuccessMessage(option(string));

  type state = {
    message: option((string, Snackbar.notificationType)),
  };

  let dispatch = ref(None);

  let component = ReasonReact.reducerComponent(__MODULE__);
  let make = _children => {
    ...component,

    initialState: () => {
      message: None
    },

    reducer: (action, _state: state) => 
      switch(action){
      | SetWarningMessage(warningMessage) => { 
        ReasonReact.Update({ 
          message: warningMessage |. Belt.Option.map(message => (message, Snackbar.Warning)) 
        })}
      | SetErrorMessage(errorMessage) => {
        ReasonReact.Update({
          message: errorMessage |. Belt.Option.map(message => (message, Snackbar.Error))
        })}
      | SetSuccessMessage(successMessage) => {
        ReasonReact.Update({
          message: successMessage |. Belt.Option.map(message => (message, Snackbar.Success))
        })
      }
    },

    /**
     * we expose dispatch to epics which are external to this component
     * this also assumes there is only a single instance of this component attached at any given time
     */
    didMount: self => dispatch := Some(self.send),
    willUnmount: _self => dispatch := None,

    render: ({ state, send }) => {
      let message = state.message |. Belt.Option.map(((message, _messageType)) => message);
      let type_ = state.message |. Belt.Option.map(((_message, messageType)) => messageType);

      <Fragment> 
        <Snackbar 
          ?type_
          key=(message |? "message") 
          isOpen=(!?state.message)
          ?message
          onExited=(_event => {
            send(SetSuccessMessage(None));
          })/>
      </Fragment>
    }
  };

  module Jsx3 = {
    [@bs.obj] external makeProps: (~dummy: string, unit) => _ = "";
    let make =
      ReasonReactCompat.wrapReasonReactForReact(
        ~component, (reactProps: {. "dummy": string}) =>
        make([||])
      );
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
         (error |. Amplify.Error.codeGet != "CodeMismatchException")
      && (error |. Amplify.Error.codeGet != "ExpiredCodeException") => Some((error, Context.dispatch^)) 
    | _ => None)
  |> optMap(fun | (error, Some(dispatch)) => Some((error, dispatch)) | _ => None)
  |> tap(~next=((error: Amplify.Error.t, dispatch)) => 
    dispatch(
      error|.Amplify.Error.codeGet == "CRITICAL" 
        ? Context.SetErrorMessage(Some(error|.Amplify.Error.messageGet))
        : Context.SetWarningMessage(Some(error|.Amplify.Error.messageGet))
    ))
  |> empty;
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
  |> empty;
};

let epic = reduxObservable => 
  Rx.Observable.merge([|
    reduxObservable |. cognitoErrors,
    reduxObservable |. cognitoSuccesses
  |]);
