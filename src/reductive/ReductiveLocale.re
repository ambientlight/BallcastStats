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

let enhancer = (storeCreator: Reductive.storeCreator('action, 'origin, 'state)) => (~reducer, ~preloadedState, ~enhancer=?, ()) => {
  let store = storeCreator(~reducer, ~preloadedState, ~enhancer?, ());
  Reductive.Store.replaceReducer(store, localeReducer(Obj.magic(store).reducer));
  store
};