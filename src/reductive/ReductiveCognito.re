type cognitoUser;

/* naked store type so we can access the reducer */
type _reductiveState('action, 'state) = {
  mutable state: 'state,
  mutable reducer: ('state, 'action) => 'state,
  mutable listeners: list(unit => unit),
  customDispatcher:
    option((_reductiveState('action, 'state), 'action => unit, 'action) => unit),
};

type withAuth('state) = {
  user: option(cognitoUser),
  state: 'state
};

type cognitoAction = [
  | `SignIn(string as 'username, string as 'password)
];

let cognitoReducer = reducer => (state, action) =>
  switch(action){
  | `SignIn(_, _) => {
    ...state,
    state: reducer(state.state, action)
  }
  | _ => { 
    ...state,
    state: reducer(state.state, action)
  }};

let enhancer = (storeCreator: Reductive.storeCreator('action, 'origin, 'state)) => (~reducer, ~preloadedState, ~enhancer=?, ()) => {
  let store = storeCreator(~reducer, ~preloadedState, ~enhancer?, ());
  Reductive.Store.replaceReducer(store, cognitoReducer(Obj.magic(store).reducer));
  store
};