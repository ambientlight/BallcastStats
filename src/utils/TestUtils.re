let fullfillIn = (callback, observable) => 
  observable 
  |> Rx.Observable.subscribe(
    ~next=fulfilled => callback(fulfilled),
    ~error=failed => callback(failed));

let testObservable = (name, observableCallback) => 
  Jestio.testAsync(name, callback => 
    observableCallback() 
    |> fullfillIn(callback));

let actionRecordingStore = middleware =>  
  Reductive.Store.create(
    ~reducer=(state, action) => state @ [action],
    ~preloadedState=[],
    ~enhancer=ReductiveObservable.middleware(
      Rx.Observable.of1(middleware)
    ),
    ());

let observableActionRecordingStore = middleware => {
  /* make sure we can hold enough actions prior to initial subscription */
  let eval = Rx.ReplaySubject.make(1024);
  let store = actionRecordingStore(
    redObs => Rx.Observable.merge([|
      redObs |. middleware,
      redObs
      |> Rx.Observable.Operators.tap(~next=actionAndState=>Rx.ReplaySubject.next(eval, actionAndState))
      |> ReductiveObservable.Utils.empty
    |])
  );
  (eval |> Rx.ReplaySubject.asObservable, store)
};

/**
 * needed to avoid: 
 * [React Intl] Could not find required `intl` object. 
 * <IntlProvider> needs to exist in the component ancestry. 
 */
let withIntl = element => 
  <ReactIntl.IntlProvider locale="en">element</ReactIntl.IntlProvider>
