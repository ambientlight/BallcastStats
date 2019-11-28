[@bs.config {jsx: 3}];

module AppStore = {
  include ReductiveContext.Make({
    type action = Store.Action.t;
    type state = Store.state;
  });
};

let forceSignInUrl: ReasonReact.Router.url = { path: ["sign-in"], hash: "", search: "" };
let routeSelector = (state: Store.state) => 
  Env.signInRequired && !(switch(state.state.user){ | SignedIn(_user) => true | _ => true })
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
      // | ["typography-test", ..._] => <TypographyTest/>
      // | ["formation-test", ..._] => <FormationTest/>
      | _ => <Landing dispatch title locale/>
    };
  }
};

//FIXME: bs-react-intl is not on jsx3 yet
module IntlProvider = {
  [@bs.module "react-intl"][@react.component]
  external make: (
    ~locale: string=?,
    ~formats: option(Js.t({..}))=?, /* TODO */
    ~messages: Js.Dict.t(string)=?,
    ~defaultLocale: option(string)=?,
    ~defaultFormats: option(Js.t({..}))=?, /* TODO */
    //~textComponent: option(textComponent)=?,
    ~initialNow: option(int)=?,
    //~onError: option(errorHandler)=?,
    ~children: 'children
  ) => React.element = "IntlProvider"
}

//FIXME: seems there jsx3 for bs-material-ui but we still have tons of logic with gsx2 underneath
module ThemeProvider = {
  [@bs.module "@material-ui/core"][@react.component]
  external make: (
    ~disableStylesGeneration: bool=?,
    ~sheetsCache: Js.t({..})=?,
    ~sheetsManager: Js.t({..})=?,
    ~theme: MaterialUi_Theme.t,
    ~children: 'children
  ) => React.element = "MuiThemeProvider"
};

module Root = {
  [@react.component]
  let make = (~title: string) => {
    let locale = AppStore.useSelector(localeSelector);

    <IntlProvider 
      locale=(locale|.Locale.toString)
      messages=(locale|.Locale.toMessages|.ReactIntl.messagesArrayToDict)>

        <ThemeProvider theme=AppTheme.theme> 
          <Shell title locale/>
          <NotificationEpics.Context/>
        </ThemeProvider>

    </IntlProvider>
  };
};

[@react.component]
let make = () => 
  <AppStore.Provider store=Store.store>
    <Root title="tracereplay"/>
  </AppStore.Provider>