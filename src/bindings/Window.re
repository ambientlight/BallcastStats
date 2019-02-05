type event = Js.t({.});

[@bs.val][@bs.scope "window"]
external addEventListener: (
  [@bs.string] [ 
    | `resize(event => unit)
  ]
) => unit = "addEventListener";

[@bs.val][@bs.scope "window"]
external innerWidth: int = "innerWidth";  

[@bs.val][@bs.scope "window"]
external innerHeight: int = "innerHeight";  
