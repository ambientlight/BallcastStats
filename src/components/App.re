module App = {
  let component = ReasonReact.statelessComponent("App");
  let make = _children => {
    ...component,
    render: _self =>
    <MaterialUi.Typography variant=`Display1>
      (ReasonReact.string("BallcastStats"))
    </MaterialUi.Typography>
      ,
  };

  let jsComponent = ReasonReact.wrapReasonForJs(~component, _ => make([||]));
};