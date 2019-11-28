open ReductiveDevTools.Utilities;

/* naked store type so we can access the reducer */
type _reductiveState('action, 'state) = {
  mutable state: 'state,
  mutable reducer: ('state, 'action) => 'state,
  mutable listeners: list(unit => unit),
  customDispatcher:
    option((_reductiveState('action, 'state), 'action => unit, 'action) => unit),
};

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

let enhancer = (storeCreator: ReductiveT.storeCreator('action, 'state)) => (~reducer, ~preloadedState, ~enhancer=?, ()) => {
  let interceptPushRoute = (store, next, action) => {
    switch(enhancer){
    | Some(enhancer) => enhancer(store, next, action)
    | None => next(action)
    };
    
    switch(action){
    | `RouterPushRoute(path) => ReasonReact.Router.push(path)
    | _ => ()
    };
  };
  
  let store = storeCreator(~reducer, ~preloadedState, ~enhancer=interceptPushRoute, ());
  let _watcherId = ReasonReact.Router.watchUrl(url => 
    Reductive.Store.dispatch(
      store, 
      `RouterLocationChanged({ ...url, path: url.path |. tagList } |. tagRecord([|"path", "hash", "search"|])
  )));

  /* Webapi.Dom.window |> WindowRe.addEventListener("popstate", event => {
    Js.log(event);
  }); */
  
  Reductive.Store.replaceReducer(store, routerReducer(Obj.magic(store).reducer));
  store
};

