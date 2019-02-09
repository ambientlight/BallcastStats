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

let epic = (action: Rx.Observable.t('action), state: Rx.Observable.t('state)) => Rx.({
  action 
  |. Observable.filter(action => switch(action){ | `RouterLocationChanged(_) => true | _ => false })
  |> Observable.tap(~next=action => {
    ~~"test96";
    ~~action;
    ()
  })
  |. Observable.map(_action => `DummySetSession("test6"))
});

let observableMiddleware = (rootEpic) => Rx.({
  let actionSubject = Subject.make();
  let stateSubject = Subject.make();
  /**
   * hack since next function subscription call actually appears
   * after the point when all middleware parameter got applied */
  let didSubscribe = ref(false);

  (store: Reductive.Store.t('action, 'state)) => {
    
    if(!didSubscribe^){
      /** 
       * hack for hot reload of middleware to be working properly
       * since middleware change will reload Store.re
       * epic subscription is stored in another file so it's not reloaded (and thus we still keep track of our subscription)
       * we then resubscribe
       */
      let _ = Belt.Option.map(SubscriptionRef.value^, subscription => {
        ~~"did unsubscribe";
        Rx.Subscription.unsubscribe(subscription);
      });

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
let store = storeCreator(~reducer=appReducer, ~preloadedState=initial, ~enhancer=observableMiddleware(epic), ());
