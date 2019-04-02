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
    ~reducer=(state, action) => [action, ...state],
    ~preloadedState=[],
    ~enhancer=ReductiveObservable.middleware(
      Rx.BehaviorSubject.make(middleware)
      |. Rx.BehaviorSubject.asObservable
    ),
    ());