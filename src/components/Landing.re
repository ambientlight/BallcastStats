open MaterialUi;
open Css;
module Styles = LandingStyles;
module Classes = Typography.Classes;

module NavHeader {
  let style = style([
    padding2(~v=px(26), ~h=px(54))
  ]);

  let make = (~title, _children) => {
    ...ReasonReact.statelessComponent("NavHeader"),
    render: _self =>
      <div className=style>
        <Logo.WithCaption caption=title/>
      </div>
  }
};

let make = (~title, _children) => {
  ...ReasonReact.statelessComponent("Landing"),
  render: _self => 
    <div className=Styles.root>
      <NavHeader title/>

      /* <Typography variant=`Display1 classes=[Classes.Root(Styles.typography)]>
        (ReasonReact.string(""))
      </Typography> */
    </div>
};