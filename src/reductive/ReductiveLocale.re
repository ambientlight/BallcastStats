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

let enhancer = (storeCreator) => (~reducer, ~preloadedState, ~enhancer=?, ()) =>
  storeCreator(
    ~reducer=localeReducer @@ reducer, 
    ~preloadedState, 
    ~enhancer?,
    ());