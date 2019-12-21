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

let routerReducer = reducer => (state, action) => 
  switch(action){
  | `RouterLocationChanged(route) => { 
    ...state, 
    route
  }
  | _ => {
    ...state,
    state: reducer(state.state, action)
  }};

let interceptPushRoute = (store, next, action) => {
  next(action);
  switch(action){
  | `RouterPushRoute(path) => ReasonReact.Router.push(path)
  | _ => ()
  };
}

let enhancer = storeCreator => (~reducer, ~preloadedState, ~enhancer=?, ()) => { 
  let store = storeCreator(
    ~reducer = routerReducer @@ reducer, 
    ~preloadedState, 
    ~enhancer=?Some(
      enhancer
      |.Belt.Option.mapWithDefault(
        interceptPushRoute,
        middleware => ((store, next) => interceptPushRoute(store) @@ middleware(store) @@ next)
      )),
    ());

  ReasonReact.Router.watchUrl(url => 
    Reductive.Store.dispatch(
      store, 
      `RouterLocationChanged({ ...url, path: url.path |. tagList } |. tagRecord([|"path", "hash", "search"|])
  ))) |> ignore;
  
  store
};

