open Operators;
open Rx.Observable.Operators;
open ReductiveObservable.Utils;

let epic = (reduxObservable: Rx.Observable.t(('action, 'state))) => {
  NotificationEpics.epic(reduxObservable);
}