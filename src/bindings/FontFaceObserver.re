type t;

[@bs.deriving abstract]
type options = {
  [@bs.optional] weight: int,
};

[@bs.module][@bs.new]
external create: (~fontFamily: string, ~options: options=?, unit) => t = "fontfaceobserver";

[@bs.send]
external load: (t) => Js.Promise.t(unit) = "load";