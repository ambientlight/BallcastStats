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
  | SignedIn(Amplify.Auth.CognitoUser.t)
  | SignInError(Amplify.Error.t)
  | SignUpError(Amplify.Error.t)
  | AccountVerificationError(Amplify.Error.t, string as 'code, string as 'username);

type withAuth('state) = {
  user: signInState,
  state: 'state
};

type cognitoAction = [
  | `SignInRequest(string as 'username, string as 'password)
  | `SignInStarted(unit)
  | `SignInCompleted(Amplify.Auth.CognitoUser.t)
  | `SignInError(Amplify.Error.t, string as 'username)

  | `CompleteNewPasswordRequest(string as 'password)
  | `CompleteNewPasswordRequestStarted(unit)
  | `CompleteNewPasswordRequestCompleted(Amplify.Auth.CognitoUser.t)
  | `CompleteNewPasswordRequestError(Amplify.Error.t)

  | `SignUpRequest(string as 'username, string as 'password)
  | `SignUpRequestRejected(Amplify.Error.t)
  | `SignUpStarted(unit)
  | `SignUpCompleted(Amplify.Auth.SignUpResult.t)
  | `SignUpError(Amplify.Error.t)
  
  | `ConfirmSignUpRequest(string as 'code, string as 'username)
  | `ConfirmSignUpStarted(string as 'code, string as 'username)
  | `ConfirmSignUpCompleted(Js.t({.}))
  | `ConfirmSignUpError(Amplify.Error.t, string as 'code, string as 'username)

  | `ResendVerificationRequest(string as 'username)
  | `ResendVerificationStarted(string as 'username)
  | `ResendVerificationCompleted(Js.t({.}), string as 'username)
  | `ResendVerificationError(Amplify.Error.t) 
  
  | `SignOutRequest(unit)
  | `SignOutStarted(unit)
  | `SignOutCompleted(unit)
  | `SignOutError(Amplify.Error.t)
  
  | `ForceVerificationRequired(string as 'code, string as 'username)
];

/** 
 * misconfigured amplify will result in error not wrapping Amplify.Error.t, 
 * rather a string, convert it to expected Amplify.Error.t
 */
let composeError = error => switch(Js.Types.classify(error)){
  | JSUndefined 
  | JSSymbol(_)
  | JSFunction(_)
  | JSNull => Amplify.Error.t(~code="CRITICAL", ~name="Unknown Error", ~message="Unknown Error")
  | JSTrue => Amplify.Error.t(~code="CRITICAL", ~name="Unknown Error(True)", ~message="Unknown Error(True)")
  | JSFalse => Amplify.Error.t(~code="CRITICAL", ~name="Unknown Error(False)", ~message="Unknown Error(False)")
  | JSString(value) => Amplify.Error.t(~code="CRITICAL",~name=value, ~message=value)
  | JSNumber(value) => Amplify.Error.t(~code=string_of_float(value), ~name="Unknown Error", ~message="Unknown Error")
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
    user: error |. Amplify.Error.codeGet == "UserNotConfirmedException" 
      ? AccountVerificationRequired("", username)
      : SignInError(error),
    state: reducer(state.state, action)
  }
  | `SignUpCompleted(signUpResult) => {
    user: signUpResult |. Amplify.Auth.SignUpResult.userConfirmedGet 
      ? SignedIn(signUpResult |. Amplify.Auth.SignUpResult.userGet)
      : AccountVerificationRequired("",
        signUpResult
        |. Amplify.Auth.SignUpResult.userGet
        |. Amplify.Auth.CognitoUser.usernameGet
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
  | `ForceVerificationRequired(code, username) => {
    user: AccountVerificationRequired(code, username),
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
  let signIn = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Observable.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`SignInRequest(username, password), _) => Some((username, password)) | _ => None)
    |> mergeMap(((username, password)) => 
      Rx.Observable.merge([|
        Rx.Observable.of1(`SignInStarted(())),
        Amplify.Auth.signIn(~username, ~password)
        |> Rx.Observable.fromPromise
        |> map(user => `SignInCompleted(user))
        |> catchError(error => Rx.Observable.of1(`SignInError(error|.composeError, username)))
      |]))
  });

  let completeNewPassword = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Observable.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`CompleteNewPasswordRequest(password), state) => Some((state.user, password)) | _ => None)
    |> Utils.Rx.optMap(fun | (SignedIn(user), password) => Some((user, password)) | _ => None)
    |> mergeMap(((user, password)) => 
      Rx.Observable.merge([|
        Rx.Observable.of1(`CompleteNewPasswordRequestStarted()),
        Amplify.Auth.completeNewPassword(~user, ~password, ())
        |> Rx.Observable.fromPromise
        |> map(target => `CompleteNewPasswordRequestCompleted(target))
        |> catchError(error => Rx.Observable.of1(`CompleteNewPasswordRequestError(error|.composeError)))
      |]))
  });

  let signUp = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Observable.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`SignUpRequest(username, password), _state) => Some((username, password)) | _ => None)
    |> mergeMap(((username, password)) => 
      Rx.Observable.merge([|
        Rx.Observable.of1(`SignUpStarted(())),
        Amplify.Auth.signUp(
          ~params=Amplify.Auth.SignUpParams.t(
            ~username, 
            ~password,
            /* next attribute is not needed when userPool allows emails as usernames */
            /* ~attributes=Js.Dict.fromList([("email", username)]),  */
            ()))
        |> Rx.Observable.fromPromise
        |> map(signUpResult => `SignUpCompleted(signUpResult))
        |> catchError(error => Rx.Observable.of1(`SignUpError(error|.composeError)))
      |]))
  });

  let confirmSignUp = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Observable.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`ConfirmSignUpRequest(code, username), _state) => Some((code, username)) | _ => None)
    |> mergeMap(((code, username)) => 
      Rx.Observable.merge([|
        Rx.Observable.of1(`ConfirmSignUpStarted(code, username)),
        Amplify.Auth.confirmSignUp(~username, ~code, ())
        |> Rx.Observable.fromPromise
        |> map(result => `ConfirmSignUpCompleted(result))
        |> catchError(error => Rx.Observable.of1(`ConfirmSignUpError(error|.composeError, code, username)))
      |]))
  });

  let resendSignUp = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Observable.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`ResendVerificationRequest(username), _state) => Some(username) | _ => None)
    |> mergeMap(username =>
      Rx.Observable.merge([|
        Rx.Observable.of1(`ResendVerificationStarted(username)),
        Amplify.Auth.resendSignUp(~username)
        |> Rx.Observable.fromPromise
        |> map(result => `ResendVerificationCompleted(result, username))
        |> catchError(error => Rx.Observable.of1(`ResendVerificationError(error|.composeError)))
      |])
    )
  })

  let signOut = (reductiveObservable: Rx.Observable.t(('action, 'state))) => Rx.Observable.Operators.({
    reductiveObservable
    |> Utils.Rx.optMap(fun | (`SignOutRequest(()), _state) => Some(()) | _ => None)
    |> mergeMap(_ => 
      Rx.Observable.merge([|
        Rx.Observable.of1(`SignOutStarted(())),
        Amplify.Auth.signOut(())
        |> Rx.Observable.fromPromise
        |> map(_result => `SignOutCompleted(()))
        |> catchError(error => Rx.Observable.of1(`SignOutError(error|.composeError)))
      |])
    )
  })

  let root = reductiveObservable => Rx.Observable.Operators.({
    Rx.Observable.merge([|
      reductiveObservable|.signIn,
      reductiveObservable|.completeNewPassword,
      reductiveObservable|.signUp,
      reductiveObservable|.confirmSignUp,
      reductiveObservable|.resendSignUp,
      reductiveObservable|.signOut
    |])
  });
};

let enhancer = (storeCreator: Reductive.storeCreator('action, 'origin, 'state)) => (~reducer, ~preloadedState, ~enhancer, ()) => {
  let withCognitoEpics = (store, next, action) =>
    ReductiveObservable.middleware(
      Rx.Observable.of1(Epics.root),
      store, 
      switch(enhancer){ | Some(enhancer) => enhancer(store, next) | None => next }, 
      action);
  
  let store = storeCreator(~reducer, ~preloadedState, ~enhancer=withCognitoEpics, ());
  Reductive.Store.replaceReducer(store, cognitoReducer(Obj.magic(store).reducer));
  store
};