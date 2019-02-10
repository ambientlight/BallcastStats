type t('a);

[@bs.module "rxjs"] [@bs.new] external make : 'a => t('a) = "BehaviorSubject";

[@bs.send] 
external next: (t('a), 'a) => unit = "next";

external asObservable : t('a) => Rx_Observable.t('a) = "%identity";

external asObserver : t('a) => Rx_Observer.t('a) = "%identity";