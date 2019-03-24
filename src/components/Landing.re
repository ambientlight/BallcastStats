open Operators;

module TopCTA {
  module Styles = LandingStyles.TopCTA;
  let make = (~className=?, _children) => {
    ...ReasonReact.statelessComponent(__MODULE__),
    render: _self => 
      <div className=([Styles.root, className |? ""] >|< " ")>
        <MaterialUi.Typography variant=`H3 className=Styles.title>
          {ReasonReact.string("Enhance your")}
          <br/>
          {ReasonReact.string("football publications.")}
        </MaterialUi.Typography>
        <MaterialUi.Typography variant=`Subtitle1 className=Styles.subtitle>
          {ReasonReact.string("Quickly build stats and assemble formations.")}
          <br/>
          {ReasonReact.string("Embed it directly into your publication.")}
        </MaterialUi.Typography>
        <MaterialUi.Button className=([AuthStyles.button, Styles.ctaBlendedButton] >|< " ")>{"Try now for free"}</MaterialUi.Button>
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
let make = (~title, ~dispatch: 'action => unit, _children) => {
  ...ReasonReact.statelessComponent(__MODULE__),
  render: _self => 
    <div className=Styles.root>
      <NavHeader dispatch title className=Styles.navHeader/>
      <TopCTA className=Styles.topCTA/>
      <TopPreview className=Styles.topPreview/>
    </div>
};