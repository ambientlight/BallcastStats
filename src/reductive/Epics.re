open Operators;

let epic = (action: Rx.Observable.t('action), state: Rx.Observable.t('state)) => 
  action 
  |. Rx.Observable.filter(action => switch(action){ | `RouterLocationChanged(_) => true | _ => false })
  |> Rx.Observable.tap(~next=action => {
    ~~"test";
    ~~action;
    ()
  })
  |. Rx.Observable.map(_action => `DummySetSession("test"));