/* [%%debugger.chrome] */

module App = {
  let component = ReasonReact.statelessComponent("App");
  let make = _children => {
    ...component,
    render: _self =>
      <div>
        (ReasonReact.string("Hi from reason"))
      </div>
      ,
  };

  let jsComponent = ReasonReact.wrapReasonForJs(~component, _ => make([||]));
};