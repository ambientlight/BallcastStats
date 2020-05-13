type observableAction('action) =
  | Start('action, Rx.BehaviorSubject.t('action))
  | Update('action, Rx.BehaviorSubject.t('action))
  | Error('action, Rx.BehaviorSubject.t('action))
  | End('action, Rx.BehaviorSubject.t('action));

type t('action, 'state) = Reductive.Store.t(
  observableAction('action),
  Reductive.Store.t('action, 'state)
);

let observe = (store: t('action, 'state), action: 'action) => {
  let subject = Rx.BehaviorSubject.create(action);
  store |. Reductive.Store.dispatch(Start(action, subject));
  subject
}

let create = (store, ~enhancer=?, ()): t('action, 'state) => {
  Reductive.Store.create(
    ~reducer=(store, action) => {
      switch(action){
      | Start(action, _subject) => {
        Reductive.Store.dispatch(store, action)
      }
      | Update(action, subject) => {
        subject |> Rx.BehaviorSubject.next(action);
        Reductive.Store.dispatch(store, action);
      }
      | Error(action, subject) => {
        subject |> Rx.BehaviorSubject.error(action);
        Reductive.Store.dispatch(store, action);
      }
      | End(action, subject) => {
        Js.log("END");
        Js.log(action)
        subject |> Rx.BehaviorSubject.next(action);
        subject |> Rx.BehaviorSubject.complete;
        Reductive.Store.dispatch(store, action);
      }}

      store
    }, 
    ~preloadedState=store, 
    ~enhancer?,
    ()
  )
};

let unsubscribe = (store: t('action, 'state), listener, ()) => 
  store
  |. Reductive.Store.getState
  |. Reductive.Store.unsubscribe(listener, ());

let subscribe = (store: t('action, 'state), listener, ()) => 
  store
  |. Reductive.Store.getState
  |. Reductive.Store.subscribe(listener, ());

let nativeDispatch = (store: t('action, 'state), action) => 
  store
  |. Reductive.Store.getState
  |. Reductive.Store.nativeDispatch(action);

let dispatch = (store: t('action, 'state), action) => {
  store 
  |. observe(action) 
  |> ignore
}

let getState = (store: t('action, 'state)) => 
  store 
  |. Reductive.Store.getState
  |. Reductive.Store.getState;

let replaceReducer = (store: t('action, 'state), reducer) => 
  store
  |. Reductive.Store.getState
  |. Reductive.Store.replaceReducer(reducer);