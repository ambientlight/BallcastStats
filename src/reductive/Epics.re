[%%debugger.chrome];

open Operators;
open Rx.Observable.Operators;

let epic = (redObservable: Rx.Observable.t(('action, 'state))) => {
  redObservable 
  |> tap(~next=((action, state)) => {
    /* ~~action;
    ~~state; */
    ()
  })
  |> mergeMap(_action => Rx.Observable.empty);
}