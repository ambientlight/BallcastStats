open List;
open Css;
module Styles = LandingStyles;

module NavHeader {
  let navStyle = style([
    padding2(~v=px(26), ~h=px(54)),
    display(`flex),
    selector("&>button", [
      margin2(~v=px(0), ~h=px(12))
    ])
  ]);

  let make = (~title, _children) => {
    ...ReasonReact.statelessComponent("NavHeader"),
    render: _self =>
      <div className=navStyle>
        <Logo.WithCaption caption=title/>
        <FlexOne/>
        <Button.Blended className=Media.Classes.atLeastTablet>
          {"About Us"}
        </Button.Blended>
        <Button.Blended>
          {"Pricing"}
        </Button.Blended>
        <Button.Blended className=Media.Classes.atLeastTablet>
          {"Sign Up"}
        </Button.Blended>
        <Button.Blended>
          {"Sign In"}
        </Button.Blended>
      </div>
  }
};

let make = (~title, _children) => {
  ...ReasonReact.statelessComponent("Landing"),
  render: _self => 
    <div className=Styles.root>
      <NavHeader title/>
    </div>
};