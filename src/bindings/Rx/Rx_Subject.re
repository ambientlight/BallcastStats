type t('a);

[@bs.module "rxjs"] [@bs.new] external make : unit => t('a) = "Subject";

[@bs.send] 
external next: (t('a), 'a) => unit = "next";

external asObservable : t('a) => Rx_Observable.t('a) = "%identity";

external asObserver : t('a) => Rx_Observer.t('a) = "%identity";