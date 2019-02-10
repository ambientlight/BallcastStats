module Subscription: {
  type t;
  let unsubscribe: t => unit;
  let closed: t => bool;

  let add: (t, t) => t;
  let remove: (t, t) => unit;
};

module Observer: {
  type t('a);
  let next: (t('a), 'a) => unit;
  let error: (t('a), 'e) => unit;
  let complete: t('a) => unit;
};

module Scheduler: {
  type t;
  let queue: t;
  let asap: t;
  let async: t;
  let animationFrame: t;
};

module Observable: {
  type t('a);

  /* create */

  let make: ((Observer.t('a), unit) => unit) => t('a);

  let of1: 'a => t('a);
  let of2: ('a, 'a) => t('a);

  let empty: t('a);
  /* add scheduler */

  let fromArray: array('a) => t('a);
  let fromPromise: Js.Promise.t('a) => t('a);

  let defer: (unit => t('a)) => t('a);
  let deferPromise: (unit => Js.Promise.t('a)) => t('a);

  /* subscribe */

  let subscribe:
    (
      ~next: 'a => unit=?,
      ~error: 'e => unit=?,
      ~complete: unit => unit=?,
      t('a)
    ) =>
    Subscription.t;

  /* operators */

  module Operators {
    let tap:
    (
      ~next: 'a => unit=?,
      ~error: 'e => unit=?,
      ~complete: unit => unit=?,
      t('a)
    ) =>
    t('a);

    let map: ('a => 'b, t('a)) => t('b);
    let mapWithIndex: ((int, 'a) => 'b, t('a)) => t('b);

    let filter: ('a => bool, t('a)) => t('a);
    let filterWithIndex: ((int, 'a) => bool, t('a)) => t('a);

    let mergeMap: (~concurrent: int=?, 'a => t('b), t('a)) => t('b);
    let switchMap: ('a => t('b), t('a)) => t('b);

    let mergeMapWithIndex:
      (~concurrent: int=?, (int, 'a) => t('b), t('a)) => t('b);

    let observeOn: (~delay: int=?, Scheduler.t, t('a)) => t('a);
    /* let subscribeOn: (~delay: int=?, Scheduler.t, t('a)) => t('a); */
    
    let withLatestFrom: (t('b), t('a)) => t(('a, 'b));
  };
};

module Subject: {
  type t('a);
  let make: unit => t('a);
  let asObservable: t('a) => Observable.t('a);
  let asObserver: t('a) => Observer.t('a);

  let next: (t('a), 'a) => unit;
};

module BehaviorSubject: {
  type t('a);
  let make: 'a => t('a);
  let asObservable: t('a) => Observable.t('a);
  let asObserver: t('a) => Observer.t('a);

  let next: (t('a), 'a) => unit;
};