type t;

[@bs.module][@bs.new]
external create: (~fontFamily: string) => t = "fontfaceobserver";

[@bs.send]
external load: (t) => Js.Promise.t(unit) = "load";