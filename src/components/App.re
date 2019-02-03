module RouterProvider = {
  let make = Reductive.Lense.createMake(
    ~lense=(state: ReductiveRouter.withRouter(Store.State.t)) => state.route, 
    Store.store);
};

module Landing = {
  let make = (~title, _children) => {
    ...ReasonReact.statelessComponent("Landing"),
    render: _self => 
      <MaterialUi.Typography variant=`Display1>
        (ReasonReact.string(title))
      </MaterialUi.Typography>
  };
}

/** Base routing login goes here */
module Shell = {
  let make = (~state: ReasonReact.Router.url, ~dispatch, ~title, _children) => {
    ...ReasonReact.statelessComponent("Shell"),
    render: _self => 
      {switch(state.path){
      | _ => <Landing title/>
      }}
  };
};

/** Root component connects app shell with reductive router state */
module Root = {
  let component = ReasonReact.statelessComponent("Root");
  let make = (~title: string, _children) => {
    ...component,
    render: _self => 
      <RouterProvider component=Shell.make(~title)/>
  };

  [@bs.deriving abstract]
  type jsProps = {
    title: string
  };

  let jsComponent = ReasonReact.wrapReasonForJs(~component, jsProps => 
    make(
      ~title=jsProps|.titleGet,
      [||])
  );
};