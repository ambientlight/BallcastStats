open Operators;
open Css;

module Styles {
  let root = style([
    display(`flex),
    
    padding2(~v=px(16), ~h=px(24)),
    Media.atLeast(Media.Breakpoint.Laptop, [
      padding2(~v=px(26), ~h=px(54)),  
      selector("&>button", [
        padding2(~v=px(0), ~h=px(24))
      ]),
    ])
  ]);

  let button = style([
    Media.atMost(Media.Breakpoint.Tablet, [
      fontSize(`px(16)),
      padding2(~v=px(24), ~h=px(20))
    ] |> List.map(rule => !# rule))
  ]);
}

let make = (~title, ~dispatch: 'action => unit, ~className=?, _children) => {
  ...ReasonReact.statelessComponent(__MODULE__),
  render: _self =>
    <div className=([Styles.root, className |? ""] >|< " ")>
      <Logo.WithCaption dispatch caption=title/>
      <FlexOne/>
      <MaterialUi.Button className=([Styles.button, Media.Classes.atLeastTablet] >|< " ")>
        {"About Us"}
      </MaterialUi.Button>
      <MaterialUi.Button className=Styles.button>
        {"Pricing"}
      </MaterialUi.Button>
      <MaterialUi.Button
        className=([Styles.button, Media.Classes.atLeastTablet] >|< " ")
        onClick=((_event) => dispatch(`RouterPushRoute(Routes.signUp)))>
        {"Sign Up"}
      </MaterialUi.Button>
      <MaterialUi.Button className=Styles.button onClick=((_event) => dispatch(`RouterPushRoute(Routes.signIn)))>
        {"Sign In"}
      </MaterialUi.Button>
    </div>
};