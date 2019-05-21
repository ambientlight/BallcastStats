open Operators;
open Css;

module Styles {
  let root = style([
    display(`flex),
    alignItems(`center),
    justifyContent(`spaceAround),
    flexDirection(`column),

    width(vw(100.0)),
    height(vh(100.0)),

    Media.atMost(Media.Breakpoint.Phone, [
      backgroundColor(hsla(133, 51, 27, 1.0))
    ]),
    Media.atLeast(Media.Breakpoint.Tablet, [
      backgroundImage(url([%bs.raw "require('assets/svgs/background.svg')"])),
      backgroundSize(`cover)
    ])
  ]);
};

let component = ReasonReact.statelessComponent(__MODULE__);
let make = _children => {
  ...component,
  render: _self => 
    <div className=Styles.root>
      <FormationView/>
    </div>
}