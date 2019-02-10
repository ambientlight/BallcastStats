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
  let actionSubject = Subject.make();
  let stateSubject = Subject.make();
  /**
   * hack since next function subscription call actually appears
   * after the point when all middleware parameter got applied */
  let didSubscribe = ref(false);

  /**
   * capture the store to have it available for the HMR
   */
  let storeRef: ref(option(Reductive.Store.t('action, 'state))) = ref(None);

  if(HMR.isAvailable(HMR.module_)){
    HMR.accept(HMR.module_, "./lib/js/src/reductive/Epics.bs.js", () => {
      let hotReloadedRootEpic: (Rx.Observable.t('action), Rx.Observable.t('state)) => Rx.Observable.t('action) = [%bs.raw "require('reason/reductive/Epics.bs.js').epic"];
      let _ = Belt.Option.map(SubscriptionRef.value^, subscription => {
        Rx.Subscription.unsubscribe(subscription);
      });

      ~~storeRef;
      switch(storeRef^){
      | Some(store) => {
        let subscription = 
        hotReloadedRootEpic(actionSubject |. Subject.asObservable, stateSubject |. Subject.asObservable)
          |> Observable.subscribe(~next=Reductive.Store.dispatch(store));

        SubscriptionRef.value := Some(subscription);
        Console.info("Epics hot reloaded");
      }
      | None => Console.warn("Store ref is unavailable while expected, hot reload of epic not performed")
      };
    });
  
    HMR.decline(HMR.module_);
  };

  (store: Reductive.Store.t('action, 'state)) => {
    storeRef := Some(store);
    ~~storeRef;

    if(!didSubscribe^){
      let subscription = 
        rootEpic(actionSubject |. Subject.asObservable, stateSubject |. Subject.asObservable)
        |> Observable.subscribe(~next=Reductive.Store.dispatch(store));

      SubscriptionRef.value := Some(subscription);
      didSubscribe := true;
    };
  
    (next: 'action => unit, action: 'action) => {
      /* state update before epics recieve an action */
      next(action);
      
      Subject.next(actionSubject, action);
      Subject.next(stateSubject, Reductive.Store.getState(store));
    }
  }
});

let storeCreator = devToolsEnhancer @@ ReductiveRouter.enhancer @@ Reductive.Store.create;
let store = storeCreator(~reducer=appReducer, ~preloadedState=initial, ~enhancer=observableMiddleware(Epics.epic), ());