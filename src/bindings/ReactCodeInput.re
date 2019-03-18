[@bs.module "react-code-input"] external reactClass : ReasonReact.reactClass = "default";

[@bs.deriving abstract]
type jsProps = {
  [@bs.as "type"] type_: string,
  [@bs.optional] fields: int,
  [@bs.optional] className: string
};

let make = (~type_, ~fields, ~className: option(string)=?, children) =>
  ReasonReact.wrapJsForReason(~reactClass, ~props=jsProps(~type_, ~fields, ~className?, ()), children);