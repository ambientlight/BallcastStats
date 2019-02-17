open Rx;
open Rx.Observable.Operators;

let middleware = (rootEpicObservable) => {
  /* observables passed to our epics */
  let actionStateSubject = Subject.make();
  
  /**
   * reductive doesn't partially apply store to middleware on init 
   * on each action it gets fully applied like middleware(store, next, action)
   * so we need keep subscriptionRef to know if we already fed epic subscription to the store dispatch 
   */
  let subscriptionRef: ref(option(Rx.Subscription.t)) = ref(None);

  /**
   * A BIT TRICKY
   * epic is passed inside observable and gets fully applied inside switchMap,
   * turning epic function observable into our action observable and essentially canceling out the previous epic
   * (call it 'replaceEpics' if your want)
   * 
   * this allows us to support hot reloading of epics
   */
  let epicInstantiator: Rx.Observable.t(('action, 'state)) => Rx.Observable.t('action) = (actionStateObservable) => 
    rootEpicObservable 
    |> switchMap(epic => epic(actionStateObservable))
    |> observeOn(Scheduler.queue);

  (store: Reductive.Store.t('action, 'state), next: 'action => unit, action: 'action) => {
    if(subscriptionRef^ == None){
      let subscription = 
        epicInstantiator(actionStateSubject |. Subject.asObservable)
        |> Observable.subscribe(~next=Reductive.Store.dispatch(store));
      subscriptionRef := Some(subscription);
    };

    /* state update before epics recieve an action */
    next(action);
    Subject.next(actionStateSubject, (action, Reductive.Store.getState(store)));
  }
};

module Utils {
  let empty = observable => observable |> mergeMap((_action) => Rx.Observable.empty);
};