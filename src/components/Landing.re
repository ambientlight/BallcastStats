open Css;

module Styles {
  let root = style([
    padding(px(0)),
    backgroundImage(url([%bs.raw "require('assets/svgs/background.svg')"])),

    display(grid),
    gridTemplateRows([`auto, `minMax(`zero, `fr(1.0))]),
    gridTemplateColumns([`percent(45.0), `percent(55.0)]),
    Media.atMost(Media.Breakpoint.Tablet, [
      gridTemplateColumns([`fr(1.0)]),
    ])
  ]);
};

let make = (~title, _children) => {
  ...ReasonReact.statelessComponent("Landing"),
  render: _self => 
    <div className=Styles.root>
      <NavHeader title/>
      <div className=style([height(`px(400)), backgroundColor(orange)])></div>
      <div className=style([height(`px(400)), backgroundColor(purple)])></div>
    </div>
};