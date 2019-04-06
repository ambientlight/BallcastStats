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

let getById = (id, ~options=?, result) =>
  ReactTestingLibrary.getByText(~matcher=`Func((_text, node) => 
    (node |> Webapi.Dom.Element.id) == id), ~options?, result);

let getByClassName = (className: string, ~options=?, result) =>
  ReactTestingLibrary.getByText(~matcher=`Func((_text, node) => {
    switch(Js.Types.classify(node|.Webapi.Dom.Element.className)){
    | JSString(value) => {
      value
      |> Js.String.split(" ")
      |. Belt.Array.some(element => element == className)
    }
    | _ => false
    }
  }), ~options?, result);