open Operators;
open Rx.Operators;
open ReductiveObservable.Utils;

let epic = (reduxObservable: Rx.Observable.t(('action, 'state))) =>
  Rx.merge([|
    reduxObservable |> RoutingEpics.epic,
    reduxObservable |> NotificationEpics.epic |> Obj.magic
  |]);