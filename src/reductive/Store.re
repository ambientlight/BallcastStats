open Operators;

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
      ())
  );

let initial: ReductiveRouter.withRouter(State.t) = {
  route: ReductiveRouter.initialRoute,
  state: {
    session: ""
  }
};

let storeCreator = devToolsEnhancer @@ ReductiveRouter.enhancer @@ Reductive.Store.create;
let epicFeeder = Rx.BehaviorSubject.make(Epics.epic);
let store = storeCreator(
  ~reducer=appReducer, 
  ~preloadedState=initial, 
  ~enhancer=ReductiveObservable.middleware(epicFeeder |. Rx.BehaviorSubject.asObservable), 
  ());

if(HMR.isAvailable(HMR.module_)){
  HMR.accept(HMR.module_, "./lib/js/src/reductive/Epics.bs.js", () => {
    let hotReloadedRootEpic: (Rx.Observable.t('action), Rx.Observable.t('state)) => Rx.Observable.t('action) = [%bs.raw "require('reason/reductive/Epics.bs.js').epic"];
    epicFeeder 
    |. Rx.BehaviorSubject.next(hotReloadedRootEpic);

    Console.info("[HMR] (observableMiddleware) Epics hot reloaded");
  });

  HMR.decline(HMR.module_);
};