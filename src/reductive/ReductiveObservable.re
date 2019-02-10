open Rx;
open Rx.Observable.Operators;

let middleware = (rootEpicObservable) => {
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
   * epic is passed inside observable and gets fully applied inside switchMap,
   * turning epic function observable into our action observable and essentially canceling out the previous epic
   * (call it 'replaceEpics' if your want)
   * 
   * this allows us to support hot reloading of epics
   */
  let epicInstantiator = (actionObservable, stateObservable) => 
    rootEpicObservable 
    |> switchMap(epic => epic(actionObservable, stateObservable))
    |> observeOn(Scheduler.queue);

  (store: Reductive.Store.t('action, 'state), next: 'action => unit, action: 'action) => {
    if(subscriptionRef^ == None){
      let actionObservable = 
        actionSubject 
        |. Subject.asObservable
        |> observeOn(Scheduler.queue);
      let stateObservable = 
        stateSubject
        |. Subject.asObservable
        |> observeOn(Scheduler.queue);
      
      let subscription = 
        epicInstantiator(actionObservable, stateObservable)
        |> Observable.subscribe(~next=Reductive.Store.dispatch(store));

      subscriptionRef := Some(subscription);
    };

    /* state update before epics recieve an action */
    next(action);
    Subject.next(actionSubject, action);
    Subject.next(stateSubject, Reductive.Store.getState(store));
  }
};