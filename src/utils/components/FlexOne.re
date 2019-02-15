open Css;
let make = _children => {
  ...ReasonReact.statelessComponent(__MODULE__),
  render: _self => 
    <div className=style([flex(1)])>
    </div>
}