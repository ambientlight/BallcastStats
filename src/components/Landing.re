open Operators;
open LandingLocale;

module TopCTA {
  module Styles = LandingStyles.TopCTA;
  let make = (~className=?, _children) => {
    ...ReasonReact.statelessComponent(__MODULE__),
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
  let make = (~className=?, _children) => {
    ...ReasonReact.statelessComponent(__MODULE__),
    render: _self =>
      <div className=([Styles.root, className |? ""] >|< " ")>
      </div>
  }
};

module Styles = LandingStyles.Root;
let make = (~title, ~locale, ~dispatch: 'action => unit, _children) => {
  ...ReasonReact.statelessComponent(__MODULE__),
  render: _self => 
    <div className=Styles.root>
      <NavHeader dispatch title locale className=Styles.navHeader/>
      <TopCTA className=Styles.topCTA/>
      <TopPreview className=Styles.topPreview/>
    </div>
};