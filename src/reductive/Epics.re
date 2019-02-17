open Operators;
open Rx.Observable.Operators;
open ReductiveObservable.Utils;

let epic = (reduxObservable: Rx.Observable.t(('action, 'state))) => {
  reduxObservable 
  /* |> tap(~next=((action, state)) => {
    ~~"tap test";
    ~~action;
    ~~state;
    ()
  }) */
  |> empty
}