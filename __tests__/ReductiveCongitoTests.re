open Jestio;
open TestUtils;

/* modules to be mocked here */
[@bs.module "aws-amplify"] external auth: JestJs.moduleType = "Auth";

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
    auth |. JestJs.assignMock("signIn", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(user))
    ));
    
    let (eval, store) = observableActionRecordingStore(ReductiveCognito.Epics.signIn);
    let expectedActions = [
      `SignInRequest("testbot",""),
      `SignInStarted(()),
      `SignInCompleted(user)
    ];

    store |. Reductive.Store.dispatch(`SignInRequest("testbot", ""));
    eval
    |> Utils.Rx.optMap(fun | (`SignInCompleted(_), state) => Some(state) | _ => None)
    |> Rx.Observable.Operators.map(state =>
      expect(state) |> toEqual(expectedActions))
  });

  testObservable("completeNewPassword spawns started/complete actions", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ~challengeName="", ());
    auth |. JestJs.assignMock("completeNewPassword", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(user))
    ));

    let (eval, store) = observableActionRecordingStore(
      redObs => redObs 
      |> Rx.Observable.Operators.map(((action, state)) => (action, { ReductiveCognito.user: ReductiveCognito.SignedIn(user), state }))
      |. ReductiveCognito.Epics.completeNewPassword
    );
    let expectedActions = [
      `CompleteNewPasswordRequest("test_password_Magic"),
      `CompleteNewPasswordRequestStarted(()),
      `CompleteNewPasswordRequestCompleted(user)
    ];

    store |. Reductive.Store.dispatch(`CompleteNewPasswordRequest("test_password_Magic"));
    eval
    |> Utils.Rx.optMap(fun | (`CompleteNewPasswordRequestCompleted(_), state) => Some(state) | _ => None)
    |> Rx.Observable.Operators.map(state =>
    expect(state) |> toEqual(expectedActions))
  });

  testObservable("signUp request spawns started/complete actions", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ());
    let expectedSignUpResult = Amplify.Auth.SignUpResult.t(~user, ~userConfirmed=false);
    auth |. JestJs.assignMock("signUp", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(expectedSignUpResult))
    ));
    
    let (eval, store) = observableActionRecordingStore(ReductiveCognito.Epics.signUp);
    let expectedActions = [
      `SignUpRequest("testbot",""),
      `SignUpStarted(()),
      `SignUpCompleted(expectedSignUpResult)
    ];

    store |. Reductive.Store.dispatch(`SignUpRequest("testbot", ""));
    eval
    |> Utils.Rx.optMap(fun | (`SignUpCompleted(_), state) => Some(state) | _ => None )
    |> Rx.Observable.Operators.map(state =>
      expect(state) |> toEqual(expectedActions))
  });  

  testObservable("confirmSignUp request spawns started/complete actions", () => {
    auth |. JestJs.assignMock("confirmSignUp", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(Js.Obj.empty()))
    ));
    
    let (eval, store) = observableActionRecordingStore(ReductiveCognito.Epics.confirmSignUp);
    let expectedActions = [
      `ConfirmSignUpRequest("000000","testbot"),
      `ConfirmSignUpStarted("000000","testbot"),
      `ConfirmSignUpCompleted(Js.Obj.empty())
    ];

    store |. Reductive.Store.dispatch(`ConfirmSignUpRequest("000000", "testbot"));
    eval
    |> Utils.Rx.optMap(fun | (`ConfirmSignUpCompleted(_), state) => Some(state) | _ => None )
    |> Rx.Observable.Operators.map(state =>
      expect(state) |> toEqual(expectedActions))
  });

  testObservable("resendSignUp request spawns started/complete actions", () => {
    auth |. JestJs.assignMock("resendSignUp", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve("resend-signup-test"))
    ));

    let (eval, store) = observableActionRecordingStore(ReductiveCognito.Epics.resendSignUp);
    let expectedActions = [
      `ResendVerificationRequest("testbot"),
      `ResendVerificationStarted("testbot"),
      `ResendVerificationCompleted("resend-signup-test", "testbot")
    ];

    store |. Reductive.Store.dispatch(`ResendVerificationRequest("testbot"));
    eval
    |> Utils.Rx.optMap(fun | (`ResendVerificationCompleted(_, _), state) => Some(state) | _ => None)
    |> Rx.Observable.Operators.map(state => 
      expect(state) |> toEqual(expectedActions))
  });

  testObservable("signInOut request spawns started/complete actions", () => {
    auth |. JestJs.assignMock("signOut", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve(()))
    ));

    let (eval, store) = observableActionRecordingStore(ReductiveCognito.Epics.signOut);
    let expectedActions = [
      `SignOutRequest(()),
      `SignOutStarted(()),
      `SignOutCompleted(())
    ];

    store |. Reductive.Store.dispatch(`SignOutRequest(()));
    eval
    |> Utils.Rx.optMap(fun | (`SignOutCompleted(()), state) => Some(state) | _ => None)
    |> Rx.Observable.Operators.map(state => 
      expect(state) |> toEqual(expectedActions))
  });

  testObservable("ReductiveCognito rootEpics merges each middleware", () => {
    let user = Amplify.Auth.CognitoUser.t(~username="testbot", ~challengeName="", ());
    let expectedSignUpResult = Amplify.Auth.SignUpResult.t(~user, ~userConfirmed=false);
    auth |. JestJs.assignMock("signIn", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(user))));
    auth |. JestJs.assignMock("completeNewPassword", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(user))));
    auth |. JestJs.assignMock("signUp", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(expectedSignUpResult))));
    auth |. JestJs.assignMock("confirmSignUp", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(Js.Obj.empty()))));
    auth |. JestJs.assignMock("resendSignUp", MockJs.fn(JestJs.fn(() => Js.Promise.resolve("resend-signup-test"))));
    auth |. JestJs.assignMock("signOut", MockJs.fn(JestJs.fn(() => Js.Promise.resolve(()))));

    let (eval, store) = observableActionRecordingStore(
      redObs => redObs 
      |> Rx.Observable.Operators.map(((action, state)) => (action, { ReductiveCognito.user: ReductiveCognito.SignedIn(user), state }))
      |> ReductiveCognito.Epics.root);   
    
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

    store |. Reductive.Store.dispatch(`SignInRequest("testbot", ""));
    store |. Reductive.Store.dispatch(`CompleteNewPasswordRequest("test_password_Magic"));
    store |. Reductive.Store.dispatch(`SignUpRequest("testbot", ""));
    store |. Reductive.Store.dispatch(`ConfirmSignUpRequest("000000", "testbot"));
    store |. Reductive.Store.dispatch(`ResendVerificationRequest("testbot"));
    store |. Reductive.Store.dispatch(`SignOutRequest(()));

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