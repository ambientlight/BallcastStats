type cognitoUser;

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
  storeCreator(~reducer, ~preloadedState, ~enhancer?, ());
};