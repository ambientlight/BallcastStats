open Operators;
open Rx.Observable.Operators;

let epic = (action: Rx.Observable.t('action), state: Rx.Observable.t('state)) => 
  action 
  |> filter(action => switch(action){ | `RouterLocationChanged(_) => true | _ => false })
  |> tap(~next=action => {
    ~~"test";
    ~~action;
    ()
  })
  |> mergeMap(_action => Rx.Observable.empty);