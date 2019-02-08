open Operators;
GlobalCss.inject();

module RouterProvider = {
  let make = Reductive.Lense.createMake(
    ~lense=(state: ReductiveRouter.withRouter(Store.State.t)) => state.route, 
    Store.store);
};

/** Base routing login goes here */
module Shell = {
  let make = (~state: ReasonReact.Router.url, ~dispatch, ~title, _children) => {
    ...ReasonReact.statelessComponent("Shell"),
    render: _self => {
      {switch(state.path){
      | ["sign-in", ..._] => <Auth title mode=SignIn/>
      | ["sign-up", ..._] => <Auth title mode=SignUp/>
      | ["forgot", ..._] => <Auth title mode=ForgotPassword/>
      | _ => <Landing dispatch title/>
      }}
    }
  };
};

/** Root component connects app shell with reductive router state */
module Root = {
  let component = ReasonReact.statelessComponent("Root");
  let make = (~title: string, _children) => {
    ...ReasonReact.statelessComponent("Root"),
    render: _self => 
      <RouterProvider component=Shell.make(~title)/>
  };

  [@bs.deriving abstract]
  type jsProps = {
    title: string
  };

  let jsComponent = ReasonReact.wrapReasonForJs(~component, jsProps => 
    make(~title=jsProps|.titleGet, [||])
  );
};