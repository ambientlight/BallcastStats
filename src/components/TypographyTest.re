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
  ])
};

let component = ReasonReact.statelessComponent(__MODULE__);
let make = (_children) => {
  ...component,
  render: _self => 
    <div className=Styles.root>
      <MaterialUi.Typography variant=`H1>"H1"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`H2>"H2"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`H3>"H3"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`H4>"H4"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`H5>"H5"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`H6>"H6"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`Subtitle1>"subtitle1"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`Subtitle2>"subtitle2"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`Body1>"Body1"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`Body2>"Body2"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`Caption>"caption"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`Overline>"OVERLINE"</MaterialUi.Typography>
      <MaterialUi.Typography variant=`Button>"BUTTON"</MaterialUi.Typography>
      
      <MaterialUi.Button>"Button"</MaterialUi.Button>
      <MaterialUi.Button variant=`Outlined>"Outlined"</MaterialUi.Button>
      <MaterialUi.Fab color=`Secondary>"FAB"</MaterialUi.Fab>
      <MaterialUi.Button color=`Secondary variant=`Contained>"CONTAINED"</MaterialUi.Button>
    </div>
}