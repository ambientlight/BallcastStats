open Operators;

GlobalCss.inject();

module RouterProvider = {
  let make = Reductive.Lense.createMake(
    ~lense=(state: ReductiveLocale.withLocale(ReductiveCognito.withAuth(ReductiveRouter.withRouter(State.t)))) => state.state.state.route, 
    Store.store);
};

module LocaleProvider = {
  let make = Reductive.Lense.createMake(
    ~lense=(state: ReductiveLocale.withLocale(ReductiveCognito.withAuth(ReductiveRouter.withRouter(State.t)))) => state.locale, 
    Store.store);
};

/** Base routing login goes here */
module Shell = {
  let component = ReasonReact.statelessComponent(__MODULE__);
  let make = (~state: ReasonReact.Router.url, ~dispatch, ~title, ~locale, _children) => {
    ...component,
    render: _self => Routes.({
      switch(state.path){
      | ["sign-in", ..._] => <Auth title mode=SignIn/>
      | ["sign-up", ..._] => <Auth title mode=SignUp/>
      | ["forgot", ..._] => <Auth title mode=ForgotPassword/>
      | ["verify-sign-up", ..._] => <Auth title mode=VerifySignUp/>
      | ["typography-test", ..._] => <TypographyTest/>
      | _ => <Landing dispatch title locale/>
      }
    })
  };
};

module Inner = {
  let component = ReasonReact.statelessComponent(__MODULE__)
  let make = (~state as locale: Locale.locale, ~dispatch as _dispatch: Store.Action.t => unit, ~title: string, _children) => {
    ...component,
    render: _self =>
      <ReactIntl.IntlProvider 
        locale=(locale|.Locale.toString)
        messages=(locale|.Locale.toMessages|.ReactIntl.messagesArrayToDict)>

        <MaterialUi.ThemeProvider theme=AppTheme.theme> 
          /* connect app shell with reductive router state */
          <RouterProvider component=Shell.make(~title, ~locale)/>
          /* notification epics manages snackbars displayed based on actions dispatched */
          <NotificationEpics.Context/>
        </MaterialUi.ThemeProvider>
        
      </ReactIntl.IntlProvider>
  };
};

module Root = {
  let component = ReasonReact.statelessComponent(__MODULE__)
  let make = (~title: string, _children) => {
    ...component,
    render: _self =>
      <LocaleProvider component=Inner.make(~title)/>
  };

  [@bs.deriving abstract]
  type jsProps = {
    title: string
  };

  let jsComponent = ReasonReact.wrapReasonForJs(~component, jsProps => 
    make(~title=jsProps|.titleGet, [||])
  );
};