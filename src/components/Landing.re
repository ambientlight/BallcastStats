[@bs.config {jsx: 3}];

open Operators;
open LandingLocale;

module TopCTA {
  module Styles = LandingStyles.TopCTA;
  
  [@react.component]
  let make = (~className=?) => {
    <div className=([Styles.root, className |? ""] >|< " ")>
      <MaterialUi.Typography variant=`H3 className=Styles.title>
        <DefinedMessage message=strings##ctaMessageP1/>
        <br/>
        <DefinedMessage message=strings##ctaMessageP2/>
      </MaterialUi.Typography>
      <MaterialUi.Typography variant=`Subtitle1 className=Styles.subtitle>
        <DefinedMessage message=strings##ctaDescription1/>
        <br/>
        <DefinedMessage message=strings##ctaDescription2/>
      </MaterialUi.Typography>
      <MaterialUi.Button className=([AuthStyles.button, Styles.ctaBlendedButton] >|< " ")>
        <DefinedMessage message=strings##ctaButton/>
      </MaterialUi.Button>
    </div>
  };
};

module TopPreview {
  module Styles = LandingStyles.TopPreview;

  [@react.component]
  let make = (~className=?) => {
    <div className=([Styles.root, className |? ""] >|< " ")>
    </div>  
  }
};

module Styles = LandingStyles.Root;

[@react.component]
let make = (~title, ~locale, ~dispatch: 'action => unit) => {
  <div className=Styles.root id="Landing">
    <NavHeader dispatch title locale className=Styles.navHeader/>
    <TopCTA className=Styles.topCTA/>
    <TopPreview className=Styles.topPreview/>
  </div>
};