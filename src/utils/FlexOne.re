open Css;
let make = _children => {
  ...ReasonReact.statelessComponent("FlexOne"),
  render: _self => 
    <div className=style([flex(1)])>
    </div>
}