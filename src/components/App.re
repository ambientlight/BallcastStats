module AppStore = {
  include ReductiveContext.Make({
    type action = Store.Action.t;
    type state = Store.state;
  });
};

let forceSignInUrl: ReasonReact.Router.url = { path: ["sign-in"], hash: "", search: "" };
let routeSelector = (state: Store.state) => 
  Env.signInRequired && !(switch(state.state.user){ | SignedIn(_user) => true | _ => false })
    ? forceSignInUrl
    : state.state.state.route;

let localeSelector = (state: Store.state) => state.locale;
let authSelector = (state: Store.state) => state.state.user;

/** Base routing login goes here */
module Shell = {
  [@react.component]
  let make = (~title, ~locale) => {
    let user = AppStore.useSelector(authSelector);
    let route = AppStore.useSelector(routeSelector);
    let dispatch = AppStore.useDispatch();

    switch(route.path){
      | ["sign-in", ..._] => <Auth state=user dispatch title mode=SignIn/>
      | ["sign-up", ..._] => <Auth state=user dispatch title mode=SignUp/>
      | ["forgot", ..._] => <Auth state=user dispatch title mode=ForgotPassword/>
      | ["verify-sign-up", ..._] => <Auth state=user dispatch title mode=VerifySignUp/>
      | ["typography-test", ..._] => <TypographyTest/>
      | _ => <Landing dispatch title locale/>
    };
  }
};

module Root = {
  [@react.component]
  let make = (~title: string) => {
    let locale = AppStore.useSelector(localeSelector);

    <IntlProvider 
      locale=(locale|.Locale.toString)
      messages=(locale|.Locale.toMessages|.ReactIntl.messagesArrayToDict)>

        <MaterialUi.ThemeProvider theme=AppTheme.theme> 
          <Shell title locale/>
          <NotificationEpics.Context/>
        </MaterialUi.ThemeProvider>

    </IntlProvider>
  };
};

[@react.component]
let make = () => 
  <AppStore.Provider store=Store.store>
    <Root title="tracereplay"/>
  </AppStore.Provider>