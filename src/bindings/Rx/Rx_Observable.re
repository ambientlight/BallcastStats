type t('a);
type operatorFunction('a, 'b) = (. t('a)) => t('b);

let noop = _ => ();
let flip = (f, a, b) => f(b, a);

/* create */

[@bs.module "rxjs"] [@bs.new]
external make : ((Rx_Observer.t('a), unit) => unit) => t('a) = "Observable";

[@bs.module "rxjs"] external empty: t('a) = "EMPTY";
[@bs.module "rxjs"] external of1 : 'a => t('a) = "of";
[@bs.module "rxjs"] external of2 : ('a, 'a) => t('a) = "of";

[@bs.module "rxjs"] external fromArray : array('a) => t('a) = "from";
[@bs.module "rxjs"]
external fromPromise : Js.Promise.t('a) => t('a) = "from";

[@bs.module "rxjs"]
external fromEvent: ('element, string) => t('b) = "fromEvent";

[@bs.module "rxjs"] external defer : (unit => t('a)) => t('a) = "defer";
[@bs.module "rxjs"]
external deferPromise : (unit => Js.Promise.t('a)) => t('a) = "defer";

[@bs.module "rxjs"][@bs.variadic]
external merge: array(t('a)) => t('a) = "merge";

[@bs.module "rxjs"]
external merge2: (t('a), t('b)) => t('c) = "merge";
[@bs.module "rxjs"]
external merge3: (t('a), t('b), t('c)) => t('d) = "merge";
[@bs.module "rxjs"]
external merge4: (t('a), t('b), t('c), t('d)) => t('e) = "merge";
[@bs.module "rxjs"]
external merge5: (t('a), t('b), t('c), t('d), t('e)) => t('f) = "merge";
[@bs.module "rxjs"]
external merge6: (t('a), t('b), t('c), t('d), t('e), t('f)) => t('g) = "merge";
[@bs.module "rxjs"]
external merge7: (t('a), t('b), t('c), t('d), t('e), t('f), t('g)) => t('h) = "merge";
[@bs.module "rxjs"]
external merge8: (t('a), t('b), t('c), t('d), t('e), t('f), t('g), t('h)) => t('i) = "merge";
[@bs.module "rxjs"]
external merge9: (t('a), t('b), t('c), t('d), t('e), t('f), t('g), t('h), t('i)) => t('j) = "merge";

[@bs.module "rxjs"]
external interval: (int) => t(int) = "interval";

[@bs.module "rxjs"]
external intervalFromScheduler: (Rx_Scheduler.t) => t(int) = "interval";

/* consume */

[@bs.send]
external _subscribe :
  (
    t('a),
    [@bs.uncurry] ('a => unit),
    [@bs.uncurry] ('e => unit),
    [@bs.uncurry] (unit => unit)
  ) =>
  Rx_Subscription.t =
  "subscribe";

let subscribe = (~next=noop, ~error=noop, ~complete=noop, source) =>
  _subscribe(source, next, error, complete);