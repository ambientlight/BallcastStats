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
  | AccountVerificationRequired(string as 'username)
  | SignedIn(Amplify.Auth.CognitoUser.t)
  | SignInError(Amplify.Error.t)
  | SignUpError(Amplify.Error.t);

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
];

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
      ? AccountVerificationRequired(username)
      : SignInError(error),
    state: reducer(state.state, action)
  }
  | `SignUpCompleted(signUpResult) => {
    user: signUpResult |. Amplify.Auth.SignUpResult.userConfirmedGet 
      ? SignedIn(signUpResult |. Amplify.Auth.SignUpResult.userGet)
      : AccountVerificationRequired(
        signUpResult
        |. Amplify.Auth.SignUpResult.userGet
        |. Amplify.Auth.CognitoUser.usernameGet
      ),
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
        |> catchError((error: Amplify.Error.t) => Rx.Observable.of1(`SignInError(error, username)))
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
        |> catchError((error: Amplify.Error.t) => Rx.Observable.of1(`CompleteNewPasswordRequestError(error)))
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
        |> catchError((error: Amplify.Error.t) => Rx.Observable.of1(`SignUpError(error)))
      |]))
  });

  let root = reductiveObservable => Rx.Observable.Operators.({
    Rx.Observable.merge([|
      reductiveObservable|.signIn,
      reductiveObservable|.completeNewPassword,
      reductiveObservable|.signUp
    |])
  });
};

let enhancer = (storeCreator: Reductive.storeCreator('action, 'origin, 'state)) => (~reducer, ~preloadedState, ~enhancer, ()) => {
  let withCognitoEpics = (store, next, action) => {
    switch(enhancer){
    | Some(enhancer) => enhancer(store, next, action)
    | None => next(action)
    };

    ReductiveObservable.middleware(
      Rx.Observable.of1(Epics.root),
      store, next, action);
    ()
  };
  
  let store = storeCreator(~reducer, ~preloadedState, ~enhancer=withCognitoEpics, ());
  Reductive.Store.replaceReducer(store, cognitoReducer(Obj.magic(store).reducer));
  store
};