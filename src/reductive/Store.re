module State {
  type t = {
    session: string
  }
};

module Action = {
  type t = [
    | `DummySetSession(string) 
    | ReductiveRouter.routerActions
  ];
}

let appReducer = (state: State.t, action) => 
  switch(action){
  | `DummySetSession(sessionDummy) => {
    ...state,
    session: sessionDummy
  }
  | _ => state
  };

let devToolsEnhancer: ReductiveDevTools.Connectors.storeEnhancer(Action.t, State.t, ReductiveRouter.withRouter(State.t)) = 
  ReductiveDevTools.Connectors.reductiveEnhancer(
    ReductiveDevTools.Extension.enhancerOptions(
      ~name="BallcastStats",
      ~actionCreators=Js.Dict.fromList([]),
      ()));

let initial: ReductiveRouter.withRouter(State.t) = {
  route: ReductiveRouter.initialRoute,
  state: {
    session: ""
  }
};

let storeCreator = devToolsEnhancer @@ ReductiveRouter.enhancer @@ Reductive.Store.create;
let store = storeCreator(~reducer=appReducer, ~preloadedState=initial, ());
