open Operators;
open Css;

module Styles {
  let root = style([
    display(`flex),
    alignItems(`center),
    justifyContent(`center),
    flexDirection(`column),

    width(vw(100.0)),
    height(vh(100.0)),

    Media.atMost(Media.Breakpoint.Phone, [
      backgroundColor(hsla(133, 51, 27, 1.0))
    ]),
    Media.atLeast(Media.Breakpoint.Tablet, [
      unsafe("background", "linear-gradient( rgba(0, 0, 0, 0.65), rgba(0, 0, 0, 0.65) ), url(" ++ [%bs.raw "require('assets/svgs/background.svg')"] ++ ")"),
      backgroundSize(`cover)
    ])
  ]);
};

let component = ReasonReact.statelessComponent(__MODULE__);
let make = _children => {
  ...component,
  render: _self => 
    <div className=Styles.root>
      <FormationContainer/>
    </div>
}