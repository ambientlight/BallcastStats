open Css;

module Styles {
  let root = style([
    padding(px(0)),
    backgroundImage(url([%bs.raw "require('assets/svgs/background.svg')"])),
    
    display(grid),
    gridTemplateRows([`auto, `minMax(`zero, `fr(1.0))]),
    gridTemplateColumns([`repeat(`num(12), `fr(1.0))]),
    Media.atMost(Media.Breakpoint.Tablet, [
      gridTemplateColumns([`repeat(`num(5), `fr(1.0))]),
    ])
  ]);
};

let make = (~title, _children) => {
  ...ReasonReact.statelessComponent("Landing"),
  render: _self => 
    <div className=Styles.root>
      <NavHeader title/>
      <div className=style([
        height(`px(200)), 
        backgroundColor(orange), 
        unsafe("grid-column", "span 5")
      ])></div>
      <div className=style([
        height(`px(200)), 
        backgroundColor(purple),
        unsafe("grid-column", "span 7")
      ])></div>
    </div>

};