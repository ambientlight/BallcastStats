open Operators;
GlobalCss.inject();

module RouterProvider = {
  let make = Reductive.Lense.createMake(
    ~lense=(state: ReductiveCognito.withAuth(ReductiveRouter.withRouter(State.t))) => state.state.route, 
    Store.store);
};

/** Base routing login goes here */
module Shell = {
  let make = (~state: ReasonReact.Router.url, ~dispatch, ~title, _children) => {
    ...ReasonReact.statelessComponent(__MODULE__),
    render: _self => Routes.({
      switch(state.path){
      | ["sign-in", ..._] => <Auth title mode=SignIn/>
      | ["sign-up", ..._] => <Auth title mode=SignUp/>
      | ["forgot", ..._] => <Auth title mode=ForgotPassword/>
      | _ => <Landing dispatch title/>
      }
    })
  };
};

/** Root component connects app shell with reductive router state */
module Root = {
  let component = ReasonReact.statelessComponent(__MODULE__);
  let make = (~title: string, _children) => {
    ...ReasonReact.statelessComponent("Root"),
    render: _self =>
      <Fragment> 
        <RouterProvider component=Shell.make(~title)/>
        /* notification epics manages snackbars displayed based on actions dispatched */
        <NotificationEpics.Context/>
      </Fragment>
  };

  [@bs.deriving abstract]
  type jsProps = {
    title: string
  };

  let jsComponent = ReasonReact.wrapReasonForJs(~component, jsProps => 
    make(~title=jsProps|.titleGet, [||])
  );
};