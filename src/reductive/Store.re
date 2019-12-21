open Operators;

type state = ReductiveLocale.withLocale(
  ReductiveCognito.withAuth(
    ReductiveRouter.withRouter(
      State.t
    )
  )
);

module Action = {
  type t = [
    | `DummySetSession(string) 
    | `DevToolsUpdate(state)
    | ReductiveRouter.routerActions
    | ReductiveCognito.cognitoAction
    | ReductiveLocale.localeActions
  ];
};

let devToolsEnhancer: ReductiveDevTools.Types.storeEnhancer([`DummySetSession(string) | `DevToolsUpdate(state)], State.t, Action.t, state) =
  ReductiveDevTools.Connectors.enhancer(
    ~options=ReductiveDevTools.Extension.enhancerOptions(
      ~name="reductive",
      ~actionCreators={
        "setLocaleEn": () => `SetLocale(Locale.En),
        "setLocaleRu": () => `SetLocale(Locale.Ru)
      },
      ()),
    ~devToolsUpdateActionCreator=(devToolsState: state) => `DevToolsUpdate(devToolsState),
    ()
  ) |> Obj.magic;

let initial: state = {
  locale: Locale.En,
  state: {
    user: SignedOut(),
    state: {
      route: ReductiveRouter.initialRoute,
      state: {
        session: ""
      }
    }
  }
};

/*TODO: fix the type system back around storeCreator('action, 'origin, 'state) */
let storeCreator = devToolsEnhancer @@ !!ReductiveLocale.enhancer @@ ReductiveCognito.enhancer @@ !!ReductiveRouter.enhancer @@ Reductive.Store.create;
let epicFeeder = Rx.BehaviorSubject.make(Epics.epic);
let store = storeCreator(
  ~reducer=(!!Reducers.root), 
  ~preloadedState=initial, 
  ~enhancer=ReductiveObservable.middleware(epicFeeder |. Rx.BehaviorSubject.asObservable), 
  ());

/* hot module reloading support for reductive */
/*
if(HMR.isAvailable(HMR.module_)){
  HMR.accept(HMR.module_, "./lib/js/src/reductive/epics/Epics.bs.js", () => {
    let hotReloadedRootEpic: (Rx.Observable.t(('action, 'state))) => Rx.Observable.t(('action)) = [%bs.raw "require('reason/reductive/epics/Epics.bs.js').epic"];
    
    /**
     * this is safe ONLY WHEN epics are stateless
     * given RxJs nature, it's easy to introduce implicit states into epics
     * when using anything utilizing BehaviourSubject/ReplaySubject/shareReplay etc.
     * be VERY CAREFUL with it as it can lead to impredictable states when hot reloaded
     */
    epicFeeder 
    |. Rx.BehaviorSubject.next(hotReloadedRootEpic);
    Console.info("[HMR] (Store) ReductiveObservable epics hot reloaded");
  });

  HMR.accept(HMR.module_, "./lib/js/src/reductive/Reducers.bs.js", () => {
    let hotReloadedRootReducer = [%bs.raw "require('reason/reductive/Reducers.bs.js').root"];

    /** MAKE SURE TO APPLY ALL HIGHER-ORDER REDUCERS introduced in storeCreator constructions */
    // Reductive.Store.replaceReducer(store, 
    //   ReductiveLocale.localeReducer @@ ReductiveCognito.cognitoReducer @@ ReductiveRouter.routerReducer @@ hotReloadedRootReducer
    // );
    Console.info("[HMR] (Store) Reducers hot reloaded");
  });

  HMR.decline(HMR.module_);
};
*/