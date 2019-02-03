open MaterialUi;

module Styles = LandingStyles;
let make = (~title, _children) => {
  ...ReasonReact.statelessComponent("Landing"),
  render: _self => 
    <div className=Styles.root>
      <Typography variant=`Display1 classes=[Typography.Classes.Root(Styles.typography)]>
        (ReasonReact.string(title))
      </Typography>
    </div>
};