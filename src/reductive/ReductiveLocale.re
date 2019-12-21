/* naked store type so we can access the reducer */
type _reductiveState('action, 'state) = {
  mutable state: 'state,
  mutable reducer: ('state, 'action) => 'state,
  mutable listeners: list(unit => unit),
  customDispatcher:
      option((_reductiveState('action, 'state), 'action => unit, 'action) => unit),
};

type withLocale('state) = {
  locale: Locale.locale,
  state: 'state
};

type localeActions = [
  `SetLocale(Locale.locale)
];

let localeReducer = reducer => (state, action) =>
  switch(action){
  | `SetLocale(locale) => {...state, locale }
  | _ => {...state, state: reducer(state.state, action)}
  };

let logger = (store, next, action) => {
  Js.log(action);
  next(action);
  Js.log(Reductive.Store.getState(store));
};


let enhancer = (storeCreator) => (~reducer, ~preloadedState, ~enhancer=?, ()) => {
  let store = storeCreator(
    ~reducer=localeReducer @@ reducer, 
    ~preloadedState, 
    ~enhancer=? 
      enhancer
      |.Belt.Option.map(
        middleware => ((store, next) => logger(store) @@ middleware(store) @@ next)
      ),
    ());
  store
};