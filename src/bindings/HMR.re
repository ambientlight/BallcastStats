type hot;
[@bs.send] external _accept: (hot, string, unit => unit) => unit = "accept";
[@bs.send] external _decline: hot => unit = "decline";

[@bs.deriving abstract]
type module_type = {
  hot: Js.Undefined.t(hot)
};

[@bs.val]
external module_: module_type = "module"; 

let isAvailable = (module_) => 
  module_ 
  |. hotGet 
  |. Js.Undefined.toOption
  |. Belt.Option.isSome;

let accept = (module_, path, onHotReload) => {
  let hmr = module_ |. hotGet;
  switch(hmr |. Js.Undefined.toOption){
  | None => Console.warn("Webpack HMR is not available, accept did nothing")
  | Some(hmr) => _accept(hmr, path, onHotReload);
  };
}

let decline = (module_) => {
  let hmr = module_ |. hotGet;
  switch(hmr |. Js.Undefined.toOption){
  | None => Console.warn("Webpack HMR is not available, decline did nothing")
  | Some(hmr) => { 
    _decline(hmr); 
  }
  };  
}