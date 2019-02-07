open ReductiveDevTools.Utilities;

type withRouter('state) = {
  route: ReasonReact.Router.url,
  state: 'state
};

type routerActions = [
  | `RouterLocationChanged(ReasonReact.Router.url)
  | `RouterPushRoute(string)
];

let initialRoute = { 
  ...ReasonReact.Router.dangerouslyGetInitialUrl(),
  path: ReasonReact.Router.dangerouslyGetInitialUrl().path |. tagList
} |. tagRecord([|"path", "hash", "search"|]);

let enhancer = (storeCreator: Reductive.storeCreator('action, 'origin, 'state)) => (~reducer, ~preloadedState, ~enhancer=?, ()) => {
  let routerReducer = (state, action) => 
    switch(action){
    | `RouterLocationChanged(route) => { 
      ...state, 
      route
    }
    | _ => {
      ...state,
      state: reducer(state.state, action)
    }};
  
  let interceptPushRoute = (store, next, action) => 
    switch(action, enhancer){
    | (`RouterPushRoute(path), _) => ReasonReact.Router.push(path);
    | (_, Some(enhancer)) => enhancer(store, next, action)
    | (_, None) => next(action)
    };
  
  let store = storeCreator(~reducer=routerReducer, ~preloadedState, ~enhancer=interceptPushRoute, ());
  let _watcherId = ReasonReact.Router.watchUrl(url => 
    Reductive.Store.dispatch(
      store, 
      `RouterLocationChanged({ ...url, path: url.path |. tagList } |. tagRecord([|"path", "hash", "search"|])
  )));

  store
};

