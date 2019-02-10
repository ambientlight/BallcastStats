open Operators;
open Rx.Observable.Operators;

let epic = (action: Rx.Observable.t('action), state: Rx.Observable.t('state)) => {
  action 
  |> tap(~next=action => {
    ~~action
  })
  |> mergeMap(_action => Rx.Observable.empty);
}