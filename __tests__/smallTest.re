open Jestio;
open TestUtils;

/* modules to be mocked here */
[@bs.module "aws-amplify"] external auth: JestJs.moduleType = "Auth";

describe("ReductiveCognito", () => {
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

  testObservable("resendSignUp", () => {
    let eval = Rx.ReplaySubject.make(1);
    let store = actionRecordingStore(
      redObs => Rx.Observable.merge([|
        redObs |. ReductiveCognito.Epics.resendSignUp,
        redObs |. redObs => Rx.Observable.Operators.({
          redObs
          |> Utils.Rx.optMap(fun | (`ResendVerificationCompleted(_, _), state) => Some(state) | _ => None)
          |> Rx.Observable.Operators.tap(~next=state=> Rx.ReplaySubject.next(eval, state))
          |> ReductiveObservable.Utils.empty})
      |])
    );
    
    auth 
    |. JestJs.assignMock("resendSignUp", MockJs.fn(
      JestJs.fn(() => Js.Promise.resolve("resend-signup-test"))
    ));

    Reductive.Store.dispatch(store, `ResendVerificationRequest("testbot"));
    eval
    |> Rx.ReplaySubject.asObservable
    |> Rx.Observable.Operators.map(state => 
      expect(state) 
      |> toEqual([
        `ResendVerificationCompleted("resend-signup-test", "testbot"), 
        `ResendVerificationStarted("testbot"), 
        `ResendVerificationRequest("testbot")]))
  });
  
  ()
});