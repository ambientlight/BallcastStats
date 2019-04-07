open Jest;
open TestUtils;
open Operators;
open Reductive.Store;
open ReductiveCognito;

/* modules to be mocked here */
[@bs.module "aws-amplify"] external auth: moduleType = "Auth";

describe("ReductiveCognito epics", () => {
  open Expect;
  
  /* examples
  test("mock calls", () => {
    let mock = JestJs.fn(a => string_of_int(a));
    let fn = MockJs.fn(mock);
    fn(74) |> ignore;
    fn(89435) |> ignore;

    expect(mock |> MockJs.calls)
    |> toEqual([|74, 89435|])
  });

  test("mock results", () => {
    let mock = JestJs.fn(a => string_of_int(a));
    let fn = MockJs.fn(mock);
    fn(74) |> ignore;
    
    expect(mock |> MockJs.results)
    |> toEqual([|
      MockJs.result(~type_=MockJs.resultTypeToJs(`Return), ~value="74")
    |])
  });

  testPromise("mock explicit", () => 
    MockJs.fn(JestJs.fn(() => Js.Promise.resolve(100)))()
    |> Rx.Observable.fromPromise
    |> Rx.Observable.Operators.map(value =>
      expect(value) |> toEqual(100))
    |> Rx.Observable.toPromise
  );

  testPromise("mock implicit", () => 
    MockJs.fn(
      JestJs.inferred_fn()
      |> MockJs.mockResolvedValue(Js.Undefined.return(10))
    )
    |. MockJs.call(())
    |> Rx.Observable.fromPromise
    |> Rx.Observable.Operators.map(value => 
      expect(value) |> toEqual(10))
    |> Rx.Observable.toPromise
  );
  */

  testObservable("signIn request spawns started/complete actions", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ~challengeName="", ());
    auth |. assignMock("signIn", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(user))
    ));
    
    let (eval, store) = observableActionRecordingStore(Epics.signIn);
    let expectedActions = [
      `SignInRequest("testbot",""),
      `SignInStarted(()),
      `SignInCompleted(user)
    ];

    store |. dispatch(`SignInRequest("testbot", ""));
    eval
    |> Utils.Rx.optMap(fun | (`SignInCompleted(_), state) => Some(state) | _ => None)
    |> Rx.Observable.Operators.map(state =>
      expect(state) |> toEqual(expectedActions))
  });

  testObservable("completeNewPassword spawns started/complete actions", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ~challengeName="", ());
    auth |. assignMock("completeNewPassword", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(user))
    ));

    let (eval, store) = observableActionRecordingStore(
      redObs => redObs 
      |> Rx.Observable.Operators.map(((action, state)) => (action, { user: SignedIn(user), state }))
      |. Epics.completeNewPassword
    );
    let expectedActions = [
      `CompleteNewPasswordRequest("test_password_Magic"),
      `CompleteNewPasswordRequestStarted(()),
      `CompleteNewPasswordRequestCompleted(user)
    ];

    store |. dispatch(`CompleteNewPasswordRequest("test_password_Magic"));
    eval
    |> Utils.Rx.optMap(fun | (`CompleteNewPasswordRequestCompleted(_), state) => Some(state) | _ => None)
    |> Rx.Observable.Operators.map(state =>
    expect(state) |> toEqual(expectedActions))
  });

  testObservable("signUp request spawns started/complete actions", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ());
    let expectedSignUpResult = Amplify.Auth.SignUpResult.t(~user, ~userConfirmed=false);
    auth |. assignMock("signUp", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(expectedSignUpResult))
    ));
    
    let (eval, store) = observableActionRecordingStore(Epics.signUp);
    let expectedActions = [
      `SignUpRequest("testbot",""),
      `SignUpStarted(()),
      `SignUpCompleted(expectedSignUpResult)
    ];

    store |. dispatch(`SignUpRequest("testbot", ""));
    eval
    |> Utils.Rx.optMap(fun | (`SignUpCompleted(_), state) => Some(state) | _ => None )
    |> Rx.Observable.Operators.map(state =>
      expect(state) |> toEqual(expectedActions))
  });  

  testObservable("confirmSignUp request spawns started/complete actions", () => {
    auth |. assignMock("confirmSignUp", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(Js.Obj.empty()))
    ));
    
    let (eval, store) = observableActionRecordingStore(Epics.confirmSignUp);
    let expectedActions = [
      `ConfirmSignUpRequest("000000","testbot"),
      `ConfirmSignUpStarted("000000","testbot"),
      `ConfirmSignUpCompleted(Js.Obj.empty())
    ];

    store |. dispatch(`ConfirmSignUpRequest("000000", "testbot"));
    eval
    |> Utils.Rx.optMap(fun | (`ConfirmSignUpCompleted(_), state) => Some(state) | _ => None )
    |> Rx.Observable.Operators.map(state =>
      expect(state) |> toEqual(expectedActions))
  });

  testObservable("resendSignUp request spawns started/complete actions", () => {
    auth |. assignMock("resendSignUp", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve("resend-signup-test"))
    ));

    let (eval, store) = observableActionRecordingStore(Epics.resendSignUp);
    let expectedActions = [
      `ResendVerificationRequest("testbot"),
      `ResendVerificationStarted("testbot"),
      `ResendVerificationCompleted("resend-signup-test", "testbot")
    ];

    store |. dispatch(`ResendVerificationRequest("testbot"));
    eval
    |> Utils.Rx.optMap(fun | (`ResendVerificationCompleted(_, _), state) => Some(state) | _ => None)
    |> Rx.Observable.Operators.map(state => 
      expect(state) |> toEqual(expectedActions))
  });

  testObservable("signInOut request spawns started/complete actions", () => {
    auth |. assignMock("signOut", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(()))
    ));

    let (eval, store) = observableActionRecordingStore(Epics.signOut);
    let expectedActions = [
      `SignOutRequest(()),
      `SignOutStarted(()),
      `SignOutCompleted(())
    ];

    store |. dispatch(`SignOutRequest(()));
    eval
    |> Utils.Rx.optMap(fun | (`SignOutCompleted(()), state) => Some(state) | _ => None)
    |> Rx.Observable.Operators.map(state => 
      expect(state) |> toEqual(expectedActions))
  });

  testObservable("ReductiveCognito rootEpics merges each middleware", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ~challengeName="", ());
    let expectedSignUpResult = Amplify.Auth.SignUpResult.t(~user, ~userConfirmed=false);
    auth |. assignMock("signIn", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(user))));
    auth |. assignMock("completeNewPassword", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(user))));
    auth |. assignMock("signUp", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(expectedSignUpResult))));
    auth |. assignMock("confirmSignUp", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(Js.Obj.empty()))));
    auth |. assignMock("resendSignUp", MockJs.fn(JestJs.fn(() => Js.Promise.resolve("resend-signup-test"))));
    auth |. assignMock("signOut", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(()))));

    let (eval, store) = observableActionRecordingStore(
      redObs => redObs 
      |> Rx.Observable.Operators.map(((action, state)) => (action, { user: SignedIn(user), state }))
      |> Epics.root);   
    
    let expectedActions = [
      `SignInRequest("testbot",""),
      `SignInStarted(()),
      `SignInCompleted(user),

      `CompleteNewPasswordRequest("test_password_Magic"),
      `CompleteNewPasswordRequestStarted(()),
      `CompleteNewPasswordRequestCompleted(user),

      `SignUpRequest("testbot",""),
      `SignUpStarted(()),
      `SignUpCompleted(expectedSignUpResult),

      `ConfirmSignUpRequest("000000","testbot"),
      `ConfirmSignUpStarted("000000","testbot"),
      `ConfirmSignUpCompleted(Js.Obj.empty()),

      `ResendVerificationRequest("testbot"),
      `ResendVerificationStarted("testbot"),
      `ResendVerificationCompleted("resend-signup-test", "testbot"),

      `SignOutRequest(()),
      `SignOutStarted(()),
      `SignOutCompleted(())
    ];

    store |. dispatch(`SignInRequest("testbot", ""));
    store |. dispatch(`CompleteNewPasswordRequest("test_password_Magic"));
    store |. dispatch(`SignUpRequest("testbot", ""));
    store |. dispatch(`ConfirmSignUpRequest("000000", "testbot"));
    store |. dispatch(`ResendVerificationRequest("testbot"));
    store |. dispatch(`SignOutRequest(()));

    eval
    |> Rx.Observable.Operators.map(((action, _)) => action)
    /* |> Rx.Observable.Operators.tap(~next=action => Js.log(Symbol.Debug.symbolValue(action, `BsPolyVar))) */
    |> Rx.Observable.Operators.bufferCount(3 * 6)
    |> Rx.Observable.Operators.map(actions => {
      expect(
        actions 
        |> Array.fold_left((containsAction, action) => 
          containsAction && (expectedActions |. Belt.List.some(expected => expected == action)), true)
      ) |> toEqual(true)
    })
  })
});

describe("ReductiveCognito reducer", () => {
  open Expect;

  /**
   * reductive cognito state is independent of previous state 
   * and depends only on action passed
   */

  test("signIn started results in user SigningIn state", () => {
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());
    
    store |. dispatch(`SignInStarted());
    expect(getState(store).user) |> toEqual(SigningIn(()))
  });

  test("signIn completed results in SignedIn state when user is confirmed", () => { 
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ());
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());
    
    store |. dispatch(`SignInCompleted(user));
    expect(getState(store).user) |> toEqual(SignedIn(user))
  });

  test("signIn error sets the error state when error is not UserNotConfirmed", () => {
    let error = Amplify.Error.t(~code="SomeRandomError", ~name="SomeRandomError", ~message="");
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());

    store |. dispatch(`SignInError(error, "testbot"));
    expect(getState(store).user) |> toEqual(SignInError(error))
  });

  test("signIn error sets the AccountVerificationRequired state when error is UserNotConfirmed", () => {
    let error = Amplify.Error.t(~code="UserNotConfirmedException", ~name="UserNotConfirmedException", ~message="");
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());

    store |. dispatch(`SignInError(error, "testbot"));
    expect(getState(store).user) |> toEqual(AccountVerificationRequired("", "testbot"))
  });

  test("signUp started results in user SigningIn state", () => {
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());
    
    store |. dispatch(`SignUpStarted());
    expect(getState(store).user) |> toEqual(SigningIn(()))
  });

  test("signUp completed results in SignedIn state when user is confirmed", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ());
    let signUpResult = Amplify.Auth.SignUpResult.t(~user, ~userConfirmed=true);

    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());
        
    store |. dispatch(`SignUpCompleted(signUpResult));
    expect(getState(store).user) |> toEqual(SignedIn(user))
  });

  test("signUp completed results in AccountVerificationRequired state when user is not confirmed", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ());
    let signUpResult = Amplify.Auth.SignUpResult.t(~user, ~userConfirmed=false);

    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());
        
    store |. dispatch(`SignUpCompleted(signUpResult));
    expect(getState(store).user) |> toEqual(AccountVerificationRequired("", "testbot"))
  });

  test("signUp error sets the error state", () => {
    let error = Amplify.Error.t(~code="SomeRandomError", ~name="SomeRandomError", ~message="");
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());

    store |. dispatch(`SignUpError(error));
    expect(getState(store).user) |> toEqual(SignUpError(error))
  });  

  test("confirmSignUp started sets Verifying state", () => {
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());

    store |. dispatch(`ConfirmSignUpStarted("000000", "testbot"));
    expect(getState(store).user) |> toEqual(Verifying("000000", "testbot"))
  });

  test("confirmSignUp error sets error state", () => {
    let error = Amplify.Error.t(~code="SomeRandomError", ~name="SomeRandomError", ~message="");
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());

    store |. dispatch(`ConfirmSignUpError(error, "000000", "testbot"));
    expect(getState(store).user) |> toEqual(AccountVerificationError(error, "000000", "testbot"))
  });

  test("completeNewPassword competion signs out", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ());
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedIn(user), state: () },
      ());

    store |. dispatch(`CompleteNewPasswordRequestCompleted(user));
    expect(getState(store).user) |> toEqual(SignedOut(()))
  });

  test("forceVerificationRequired sets accounts verification state", () => {
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());

    store |. dispatch(`ForceVerificationRequired("000000", "testbot"));
    expect(getState(store).user) |> toEqual(AccountVerificationRequired("000000", "testbot"))
  });

  test("resendVerification started sets in progress state", () => {
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());

    store |. dispatch(`ResendVerificationStarted("testbot"));
    expect(getState(store).user) |> toEqual(ResendingVerification("testbot"))
  });

  test("resendVerification completed sets AccountVerificationRequired state", () => {
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedOut(()), state: () },
      ());

    store |. dispatch(`ResendVerificationCompleted(Js.Obj.empty(), "testbot"));
    expect(getState(store).user) |> toEqual(AccountVerificationRequired("", "testbot"))
  });

  test("confirmSignUp completed signs out", () => {
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: Verifying("000000", "testbot"), state: () },
      ());

    store |. dispatch(`ConfirmSignUpCompleted(Js.Obj.empty()));
    expect(getState(store).user) |> toEqual(SignedOut(()))
  });

  test("signOut completed signs out", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ());
    let store = create(
      ~reducer=cognitoReducer((state, _action) => state),
      ~preloadedState={ user: SignedIn(user), state: () },
      ());

    store |. dispatch(`SignOutCompleted(()));
    expect(getState(store).user) |> toEqual(SignedOut(()))
  });
});

describe("ReductiveCognito enhancer", () => {
  open Expect;

  let storeCreator = ReductiveCognito.enhancer @@ create;
  let reducer = ((state, action) => switch(action){
    | `Increase => state + 1
    | `Decrease => state - 1
    | _ => state
  });

  test("reductive state with enhancer signsIn", () => {
    let store = storeCreator(
      ~reducer=reducer |> Obj.magic,
      ~preloadedState={ user: SignedOut(()), state: 0 },
      ~enhancer=None,
      ()
    );

    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ());
    store |. dispatch(`SignInCompleted(user));
    expect(getState(store).user) |> toEqual(SignedIn(user))
  });

  test("reductive state with enhancer propagates inner reducer action", () => {
    let store = storeCreator(
      ~reducer=reducer |> Obj.magic,
      ~preloadedState={ user: SignedOut(()), state: 0 },
      ~enhancer=None,
      ()
    );

    store |. dispatch(`Increase);
    expect(getState(store).state) |> toEqual(1)
  });

  ()
});