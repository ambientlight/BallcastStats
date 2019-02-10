open Rx_Observable;

[@bs.module "rxjs/operators"]
external _tap :
  (
    [@bs.uncurry] ('a => unit),
    [@bs.uncurry] ('e => unit),
    [@bs.uncurry] (unit => unit)
  ) =>
  operatorFunction('a, 'a) =
  "tap";
let tap = (~next=noop, ~error=noop, ~complete=noop, source) =>
  (_tap(next, error, complete))(. source);

[@bs.module "rxjs/operators"]
external _map : ([@bs.uncurry] ('a => 'b)) => operatorFunction('a, 'b) =
  "map";
let map = (project, source) => (_map(project))(. source);

[@bs.module "rxjs/operators"]
external _mapWithIndex :
  ([@bs.uncurry] (('a, int) => 'b)) => operatorFunction('a, 'b) =
  "map";
let mapWithIndex = (project, source) =>
  (_mapWithIndex(flip(project)))(. source);

[@bs.module "rxjs/operators"]
external _filter : ([@bs.uncurry] ('a => bool)) => operatorFunction('a, 'a) =
  "filter";
let filter = (predicate, source) => (_filter(predicate))(. source);

[@bs.module "rxjs/operators"]
external _filterWithIndex :
  ([@bs.uncurry] (('a, int) => bool)) => operatorFunction('a, 'a) =
  "filter";
let filterWithIndex = (predicate, source) =>
  (_filterWithIndex(flip(predicate)))(. source);

[@bs.module "rxjs/operators"]
external _mergeMap :
  ([@bs.uncurry] ('a => t('b)), ~concurrent: int=?) =>
  operatorFunction('a, 'b) =
  "flatMap";
let mergeMap = (~concurrent=?, project, source) =>
  (_mergeMap(project, ~concurrent?))(. source);

[@bs.module "rxjs/operators"]
external _switchMap :
  ([@bs.uncurry] ('a => t('b))) =>
  operatorFunction('a, 'b) =
  "switchMap";
let switchMap = (project, source) =>
  (_switchMap(project))(. source);

[@bs.module "rxjs/operators"]
external _mergeMapWithIndex :
  ([@bs.uncurry] (('a, int) => t('b)), ~concurrent: int=?) =>
  operatorFunction('a, 'b) =
  "flatMap";
let mergeMapWithIndex = (~concurrent=?, project, source) =>
  (_mergeMapWithIndex(flip(project), ~concurrent?))(. source);

[@bs.module "rxjs/operators"]
external _observeOn: (Rx_Scheduler.t, ~delay: int=?) => operatorFunction('a, 'a) = "observeOn";
let observeOn = (~delay=?, scheduler, source) => 
  (_observeOn(scheduler, ~delay?))(. source);

/* [@bs.module "rxjs/operators"]
external _subscribeOn: (Rx_Scheduler.t, ~delay: int=?) => operatorFunction('a, 'a) = "subscribeOn";
let subscribeOn = (~delay=?, scheduler, source) => 
  (_subscribeOn(scheduler, ~delay?))(. source); */