[@bs.module "react-code-input"] external reactClass : ReasonReact.reactClass = "default";

[@bs.deriving abstract]
type jsProps = {
  [@bs.as "type"] type_: string,
  fields: int
};

let make = (~type_, ~fields, children) =>
  ReasonReact.wrapJsForReason(~reactClass, ~props=jsProps(~type_, ~fields), children);