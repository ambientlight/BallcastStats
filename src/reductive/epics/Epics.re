open Operators;
open Rx.Observable.Operators;
open ReductiveObservable.Utils;

let epic = (reduxObservable: Rx.Observable.t(('action, 'state))) =>
  Rx.Observable.merge([|
    reduxObservable |. NotificationEpics.epic,
    reduxObservable |. RoutingEpics.epic
  |]);