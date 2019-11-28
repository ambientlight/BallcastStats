open Operators;
open LandingLocale;

module TopCTA {
  module Styles = LandingStyles.TopCTA;
  let component =ReasonReact.statelessComponent(__MODULE__)
  let make = (~className=?, _children) => {
    ...component,
    render: _self => 
      <div className=([Styles.root, className |? ""] >|< " ")>
        <MaterialUi.Typography variant=`H3 className=Styles.title>
          <ReactIntl.DefinedMessage message=strings##ctaMessageP1/>
          <br/>
          <ReactIntl.DefinedMessage message=strings##ctaMessageP2/>
        </MaterialUi.Typography>
        <MaterialUi.Typography variant=`Subtitle1 className=Styles.subtitle>
          <ReactIntl.DefinedMessage message=strings##ctaDescription1/>
          <br/>
          <ReactIntl.DefinedMessage message=strings##ctaDescription2/>
        </MaterialUi.Typography>
        <MaterialUi.Button className=([AuthStyles.button, Styles.ctaBlendedButton] >|< " ")>
          <ReactIntl.DefinedMessage message=strings##ctaButton/>
        </MaterialUi.Button>
      </div>
  }
};

module TopPreview {
  module Styles = LandingStyles.TopPreview;
  let component = ReasonReact.statelessComponent(__MODULE__)
  let make = (~className=?, _children) => {
    ...component,
    render: _self =>
      <div className=([Styles.root, className |? ""] >|< " ")>
      </div>
  }
};

module Styles = LandingStyles.Root;
let component = ReasonReact.statelessComponent(__MODULE__);
let make = (~title, ~locale, ~dispatch: 'action => unit, _children) => {
  ...component,
  render: _self => 
    <div className=Styles.root id="Landing">
      <NavHeader dispatch title locale className=Styles.navHeader/>
      <TopCTA className=Styles.topCTA/>
      <TopPreview className=Styles.topPreview/>
    </div>
};

module Jsx3 = {
  [@bs.obj] external makeProps: (~title: string, ~locale: Locale.locale, ~dispatch: 'action => unit, unit) => _ = "";
  let make =
    ReasonReactCompat.wrapReasonReactForReact(
      ~component, (reactProps: {. "title": string, "locale": Locale.locale, "dispatch": 'action => unit}) =>
      make(~title=reactProps##title, ~locale=reactProps##locale, ~dispatch=reactProps##dispatch, [||])
    );
};