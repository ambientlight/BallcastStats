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
  let fromEvent: ('element, string) => t('b);

  let defer: (unit => t('a)) => t('a);
  let deferPromise: (unit => Js.Promise.t('a)) => t('a);

  [@bs.module "rxjs"][@bs.variadic]
  external merge: array(t('a)) => t('a) = "merge";

  let merge2: (t('a), t('b)) => t('c);
  let merge3: (t('a), t('b), t('c)) => t('d);
  let merge4: (t('a), t('b), t('c), t('d)) => t('e);
  let merge5: (t('a), t('b), t('c), t('d), t('e)) => t('f);
  let merge6: (t('a), t('b), t('c), t('d), t('e), t('f)) => t('g);
  let merge7: (t('a), t('b), t('c), t('d), t('e), t('f), t('g)) => t('h);
  let merge8: (t('a), t('b), t('c), t('d), t('e), t('f), t('g), t('h)) => t('i);
  let merge9: (t('a), t('b), t('c), t('d), t('e), t('f), t('g), t('h), t('i)) => t('j);

  let interval: int => t(int);
  let intervalFromScheduler: Scheduler.t => t(int);
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
    let catchError: ('error => t('a), t('a)) => t('a);

    let take: (int, t('a)) => t('a);
    let takeWhile: ('a => bool, t('a)) => t('a);
    let takeUntil: (t('other), t('a)) => t('a);
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