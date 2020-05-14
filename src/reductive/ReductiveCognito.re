/* naked store type so we can access the reducer */
type _reductiveState('action, 'state) = {
  mutable state: 'state,
  mutable reducer: ('state, 'action) => 'state,
  mutable listeners: list(unit => unit),
  customDispatcher:
    option((_reductiveState('action, 'state), 'action => unit, 'action) => unit),
};

type signInState = 
  | SignedOut(unit)
  | SigningIn(unit)
  | AccountVerificationRequired(string as 'code, string as 'username)
  | Verifying(string as 'code, string as 'username)
  | ResendingVerification(string as 'username)
  | SignedIn(AWSAmplify.Auth.CognitoUser.t)
  | SignInError(AWSAmplify.Error.t)
  | SignUpError(AWSAmplify.Error.t)
  | AccountVerificationError(AWSAmplify.Error.t, string as 'code, string as 'username);

type withAuth('state) = {
  user: signInState,
  state: 'state
};

type cognitoAction = [
  | `SignInRequest(string as 'username, string as 'password)
  | `SignInStarted(unit)
  | `SignInCompleted(AWSAmplify.Auth.CognitoUser.t)
  | `SignInError(AWSAmplify.Error.t, string as 'username)

  | `CompleteNewPasswordRequest(string as 'password)
  | `CompleteNewPasswordRequestStarted(unit)
  | `CompleteNewPasswordRequestCompleted(AWSAmplify.Auth.CognitoUser.t)
  | `CompleteNewPasswordRequestError(AWSAmplify.Error.t)

  | `SignUpRequest(string as 'username, string as 'password)
  | `SignUpRequestRejected(AWSAmplify.Error.t)
  | `SignUpStarted(unit)
  | `SignUpCompleted(AWSAmplify.Auth.SignUpResult.t)
  | `SignUpError(AWSAmplify.Error.t)
  
  | `ConfirmSignUpRequest(string as 'code, string as 'username)
  | `ConfirmSignUpStarted(string as 'code, string as 'username)
  | `ConfirmSignUpCompleted(Js.t({.}))
  | `ConfirmSignUpError(AWSAmplify.Error.t, string as 'code, string as 'username)

  | `ResendVerificationRequest(string as 'username)
  | `ResendVerificationStarted(string as 'username)
  | `ResendVerificationCompleted(Js.t({.}), string as 'username)
  | `ResendVerificationError(AWSAmplify.Error.t) 
  
  | `SignOutRequest(unit)
  | `SignOutStarted(unit)
  | `SignOutCompleted(unit)
  | `SignOutError(AWSAmplify.Error.t)
  
  | `ForceVerificationRequired(string as 'code, string as 'username)
];

/** 
 * misconfigured amplify will result in error not wrapping AWSAmplify.Error.t, 
 * rather a string, convert it to expected AWSAmplify.Error.t
 */
let composeError = error => switch(Js.Types.classify(error)){
  | JSUndefined 
  | JSSymbol(_)
  | JSFunction(_)
  | JSNull => AWSAmplify.Error.t(~code="CRITICAL", ~name="Unknown Error", ~message="Unknown Error")
  | JSTrue => AWSAmplify.Error.t(~code="CRITICAL", ~name="Unknown Error(True)", ~message="Unknown Error(True)")
  | JSFalse => AWSAmplify.Error.t(~code="CRITICAL", ~name="Unknown Error(False)", ~message="Unknown Error(False)")
  | JSString(value) => AWSAmplify.Error.t(~code="CRITICAL",~name=value, ~message=value)
  | JSNumber(value) => AWSAmplify.Error.t(~code=string_of_float(value), ~name="Unknown Error", ~message="Unknown Error")
  | JSObject(objectValue) => objectValue|.Obj.magic
}

let cognitoReducer = reducer => (state, action) =>
  switch(action){
  | `SignInStarted()
  | `SignUpStarted() => {
    user: SigningIn(),
    state: reducer(state.state, action)
  }
  | `SignInCompleted(user) => {
    user: SignedIn(user),
    state: reducer(state.state, action)
  }
  | `SignInError(error, username) => {
    user: error |. AWSAmplify.Error.codeGet == "UserNotConfirmedException" 
      ? AccountVerificationRequired("", username)
      : SignInError(error),
    state: reducer(state.state, action)
  }
  | `SignUpCompleted(signUpResult) => {
    user: signUpResult |. AWSAmplify.Auth.SignUpResult.userConfirmedGet 
      ? SignedIn(signUpResult |. AWSAmplify.Auth.SignUpResult.userGet)
      : AccountVerificationRequired("",
        signUpResult
        |. AWSAmplify.Auth.SignUpResult.userGet
        |. AWSAmplify.Auth.CognitoUser.usernameGet
      ),
    state: reducer(state.state, action)
  }
  | `ConfirmSignUpStarted(code, username) => {
    user: Verifying(code, username),
    state: reducer(state.state, action)
  }
  | `ConfirmSignUpError(error, code, username) => {
    user: AccountVerificationError(error, code, username),
    state: reducer(state.state, action)
  }
  | `SignUpError(error) => {
    user: SignUpError(error),
    state: reducer(state.state, action)
  }
  /* will pass a user but with same challenge name as it was */
  | `CompleteNewPasswordRequestCompleted(_user) => {
    user: SignedOut(),
    state: reducer(state.state, action)
  }
  | `ResendVerificationStarted(username) => {
    user: ResendingVerification(username),
    state: reducer(state.state, action)
  }
  | `ResendVerificationCompleted(_result, username) => {
    user: AccountVerificationRequired("", username),
    state: reducer(state.state, action)
  }
  | `ConfirmSignUpCompleted(_result) => {
    user: SignedOut(),
    state: reducer(state.state, action)
  }
  | `SignOutCompleted() => {
    user: SignedOut(),
    state: reducer(state.state, action)
  }
  | _ => { 
    ...state,
    state: reducer(state.state, action)
  }};

module Epics {
  let signIn = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`SignInRequest(username, password), _) => Some((username, password)) | _ => None)
    |> mergeMap(`Observable(((username, password), _idx) => 
      Rx.merge([|
        Rx.of1(`SignInStarted(())),
        Rx.from(`Promise(AWSAmplify.Auth.signIn(~username, ~password)), ())
        |> map((user, _idx) => `SignInCompleted(user))
        |> catchError((error, _not) => Rx.of1(`SignInError(error|.composeError, username)))
      |])
    ))
  });

  let completeNewPassword = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`CompleteNewPasswordRequest(password), state) => Some((state.user, password)) | _ => None)
    |> Utils.Rx.optMap(fun | (SignedIn(user), password) => Some((user, password)) | _ => None)
    |> mergeMap(`Observable(((user, password), _idx) => 
      Rx.merge([|
        Rx.of1(`CompleteNewPasswordRequestStarted()),
        Rx.from(`Promise(AWSAmplify.Auth.completeNewPassword(~user, ~password, ())), ())
        |> map((target, _idx) => `CompleteNewPasswordRequestCompleted(target))
        |> catchError((error, _notif) => Rx.of1(`CompleteNewPasswordRequestError(error|.composeError)))
      |])))
  });

  let signUp = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`SignUpRequest(username, password), _state) => Some((username, password)) | _ => None)
    |> mergeMap(`Observable(((username, password), _idx) => 
      Rx.merge([|
        Rx.of1(`SignUpStarted(())),
        `Promise(AWSAmplify.Auth.signUp(
          ~params=AWSAmplify.Auth.SignUpParams.t(
            ~username, 
            ~password,
            /* next attribute is not needed when userPool allows emails as usernames */
            /* ~attributes=Js.Dict.fromList([("email", username)]),  */
            ())))
        |. Rx.from(())
        |> map((signUpResult, _idx) => `SignUpCompleted(signUpResult))
        |> catchError((error, _notif)  => Rx.of1(`SignUpError(error|.composeError)))
      |])))
  });

  let confirmSignUp = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`ConfirmSignUpRequest(code, username), _state) => Some((code, username)) | _ => None)
    |> mergeMap(`Observable(((code, username), _idx) => 
      Rx.merge([|
        Rx.of1(`ConfirmSignUpStarted(code, username)),
        Rx.from(`Promise(AWSAmplify.Auth.confirmSignUp(~username, ~code, ())), ())
        |> map((result, _idx) => `ConfirmSignUpCompleted(result))
        |> catchError((error, _notif) => Rx.of1(`ConfirmSignUpError(error|.composeError, code, username)))
      |])))
  });

  let resendSignUp = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`ResendVerificationRequest(username), _state) => Some(username) | _ => None)
    |> mergeMap(`Observable((username, _idx) =>
      Rx.merge([|
        Rx.of1(`ResendVerificationStarted(username)),
        Rx.from(`Promise(AWSAmplify.Auth.resendSignUp(~username)), ())
        |> map((result, _idx) => `ResendVerificationCompleted(result, username))
        |> catchError((error, _notif) => Rx.of1(`ResendVerificationError(error|.composeError)))
      |])
    ))
  })

  let signOut = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`SignOutRequest(()), _state) => Some(()) | _ => None)
    |> mergeMap(`Observable((_, _idx) => 
      Rx.merge([|
        Rx.of1(`SignOutStarted(())),
        Rx.from(`Promise(AWSAmplify.Auth.signOut(())), ())
        |> map((_result, _idx) => `SignOutCompleted(()))
        |> catchError((error, _notif) => Rx.of1(`SignOutError(error|.composeError)))
      |])
    ))
  })

  let root = reductiveObservable =>
    Rx.merge([|
      // reductiveObservable|.signIn,
      // reductiveObservable|.completeNewPassword,
      // reductiveObservable|.signUp,
      // reductiveObservable|.confirmSignUp,
      // reductiveObservable|.resendSignUp,
      // reductiveObservable|.signOut
    |]);
};

let enhancer = (storeCreator) => (~reducer, ~preloadedState, ~enhancer=?, ()) => {
  let store = storeCreator(
    ~reducer=cognitoReducer @@ reducer, 
    ~preloadedState, 
    ~enhancer=?Some(
      enhancer
      |.Belt.Option.mapWithDefault(
        ReductiveObservable.middleware(Rx.of1(Epics.root)),
        middleware => ((store, next) => 
          ReductiveObservable.middleware(Rx.of1(Epics.root), store) @@ middleware(store) @@ next)
      )),
    ());
  store
};

ReductiveObservable.middleware;