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

let observableMiddleware = (rootEpic) => Rx.({
  /* observables passed to our epics */
  let actionSubject = Subject.make();
  let stateSubject = Subject.make();

  /**
   * reductive doesn't partially apply store to middleware on init 
   * on each action it gets fully applied like middleware(store, next, action)
   * so we need keep subscriptionRef to know if we already fed epic subscription to the store dispatch 
   */
  let subscriptionRef: ref(option(Rx.Subscription.t)) = ref(None);

  /**
   * A BIT TRICKY
   * epic function gets fed into a subject
   * and gets fully applied inside switchMap,
   * turning epic function observable into our action observable and essentially canceling out the previous epic
   * (call it 'replaceEpics' if your want)
   * 
   * this allows us to support hot reloading of epics
   */
  let epicFeeder = BehaviorSubject.make(rootEpic);
  let epicInstantiator = (actionObservable, stateObservable) => 
    epicFeeder 
    |. BehaviorSubject.asObservable
    |. Observable.switchMap(epic => epic(actionObservable, stateObservable));

  /* hot reloading of epics */
  if(HMR.isAvailable(HMR.module_)){
    HMR.accept(HMR.module_, "./lib/js/src/reductive/Epics.bs.js", () => {
      let hotReloadedRootEpic: (Rx.Observable.t('action), Rx.Observable.t('state)) => Rx.Observable.t('action) = [%bs.raw "require('reason/reductive/Epics.bs.js').epic"];
      epicFeeder 
      |. Rx.BehaviorSubject.next(hotReloadedRootEpic);

      Console.info("[HMR] (observableMiddleware) Epics hot reloaded");
    });
  
    HMR.decline(HMR.module_);
  };

  (store: Reductive.Store.t('action, 'state), next: 'action => unit, action: 'action) => {
    if(subscriptionRef^ == None){
      let subscription = 
        epicInstantiator(actionSubject |. Subject.asObservable, stateSubject |. Subject.asObservable)
        |> Observable.subscribe(~next=Reductive.Store.dispatch(store));
      subscriptionRef := Some(subscription);
    };

    /* state update before epics recieve an action */
    next(action);
    Subject.next(actionSubject, action);
    Subject.next(stateSubject, Reductive.Store.getState(store));
  }
});

let storeCreator = devToolsEnhancer @@ ReductiveRouter.enhancer @@ Reductive.Store.create;
let store = storeCreator(~reducer=appReducer, ~preloadedState=initial, ~enhancer=observableMiddleware(!!Epics.epic), ());