let forceSignInUrl: ReasonReact.Router.url = { path: ["sign-in"], hash: "", search: "" };
let routeSelector = (state: Store.state) => 
  Env.signInRequired && !(switch(state.state.user){ | SignedIn(_user) => true | _ => false })
    ? forceSignInUrl
    : state.state.state.route;

let localeSelector = (state: Store.state) => state.locale;

/** Base routing login goes here */
module Shell = {
  [@react.component]
  let make = (~title, ~locale) => {
    let route = AppStore.useSelector(routeSelector);
    let dispatch = AppStore.useDispatch();

    switch(route.path){
      | ["sign-in", ..._] => <Auth title mode=SignIn/>
      | ["sign-up", ..._] => <Auth title mode=SignUp/>
      | ["forgot", ..._] => <Auth title mode=ForgotPassword/>
      | ["verify-sign-up", ..._] => <Auth title mode=VerifySignUp/>
      | ["typography-test", ..._] => <TypographyTest/>
      | ["embed-test", ..._] => <EmbedTest/>
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
  </AppStore.Provider>;