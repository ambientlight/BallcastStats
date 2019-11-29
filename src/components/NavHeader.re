open Operators;
open Css;
open NavHeaderLocale;
open Utils.Tests;

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

[@react.component]
let make = (~title, ~locale, ~dispatch: 'action => unit, ~className=?) => {
  <div className=([Styles.root, className |? ""] >|< " ")>
    <Logo.WithCaption dispatch caption=title/>
    <FlexOne/>
    <MaterialUi.Button 
      className=([Styles.button, Media.Classes.atLeastTablet] >|< " ")
      onClick=(_event => dispatch(`SetLocale(locale == Locale.En ? Locale.Ru : Locale.En)))
      >
      {locale == Locale.En 
        ? <DefinedMessage message=strings##inRussian/>
        : <DefinedMessage message=strings##inEnglish/>
      }
    </MaterialUi.Button>
    // <MaterialUi.Button 
    //   className=Styles.button
    //   onClick=((_event) => dispatch(`RouterPushRoute(Routes.formationTest)))>
    //   <ReactIntl.DefinedMessage message=strings##pricing/>
    // </MaterialUi.Button>
    <MaterialUi.Button
      className=([Styles.button, Media.Classes.atLeastTablet, "test-navheader-signup-button"] >|< " ")
      onClick=((_event) => dispatch(`RouterPushRoute(Routes.signUp)))>
      <DefinedMessage message=CommonLocale.strings##signUp/>
    </MaterialUi.Button>
    <MaterialUi.Button 
      className=(Styles.button |. withTestClass("test-navheader-signin-button"))
      onClick=((_event) => dispatch(`RouterPushRoute(Routes.signIn)))>
      <DefinedMessage message=CommonLocale.strings##signIn/>
    </MaterialUi.Button>
  </div>
};