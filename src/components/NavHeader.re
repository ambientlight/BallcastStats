open Operators;
open Css;

let rootStyle = style([
  display(`flex),
  
  padding2(~v=px(16), ~h=px(24)),
  Media.atLeast(Media.Breakpoint.Laptop, [
    padding2(~v=px(26), ~h=px(54)),  
    selector("&>button", [
      padding2(~v=px(0), ~h=px(24))
    ]),
  ])
]);

let make = (~title, ~dispatch: 'action => unit, ~className=?, _children) => {
  ...ReasonReact.statelessComponent(__MODULE__),
  render: _self =>
    <div className=([rootStyle, className |? ""] >|< " ")>
      <Logo.WithCaption dispatch caption=title/>
      <FlexOne/>
      <Button.Blended className=Media.Classes.atLeastTablet>
        {"About Us"}
      </Button.Blended>
      <Button.Blended>
        {"Pricing"}
      </Button.Blended>
      <Button.Blended
        className=Media.Classes.atLeastTablet
        onClick=((_event) => dispatch(`RouterPushRoute(Routes.signUp)))
      >
        {"Sign Up"}
      </Button.Blended>
      <Button.Blended onClick=((_event) => dispatch(`RouterPushRoute(Routes.signIn)))>
        {"Sign In"}
      </Button.Blended>
    </div>
};