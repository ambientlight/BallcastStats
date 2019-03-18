[@bs.module "react-code-input"] external reactClass : ReasonReact.reactClass = "default";

[@bs.deriving abstract]
type jsProps = {
  [@bs.optional][@bs.as "type"] type_: string,
  [@bs.optional] value: string,
  [@bs.optional] fields: int,
  [@bs.optional] className: string,
  [@bs.optional] disabled: bool,
  [@bs.optional] autoFocus: bool,
  [@bs.optional] onChange: string => unit
};

let make = (
  ~type_: option(string)=?,
  ~value: option(string)=?,
  ~fields: option(int)=?, 
  ~className: option(string)=?,
  ~disabled: option(bool)=?,
  ~autoFocus: option(bool)=?,
  ~onChange: option(string => unit)=?, 
  children) =>
    ReasonReact.wrapJsForReason(~reactClass, ~props=jsProps(~type_?, ~value?, ~fields?, ~className?, ~disabled?, ~autoFocus?, ~onChange?, ()), children);