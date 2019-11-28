[@bs.config {jsx: 3}];

module AppStore = {
  include ReductiveContext.Make({
    type action = Store.Action.t;
    type state = Store.state;
  });
};

/** Base routing login goes here */
module Shell = {
  [@react.component]
  let make = (~state: ReasonReact.Router.url, ~dispatch, ~title, ~locale) => 
    switch(state.path){
      | ["sign-in", ..._] => <Auth.Jsx3 title mode=SignIn/>
      | ["sign-up", ..._] => <Auth.Jsx3 title mode=SignUp/>
      | ["forgot", ..._] => <Auth.Jsx3 title mode=ForgotPassword/>
      | ["verify-sign-up", ..._] => <Auth.Jsx3 title mode=VerifySignUp/>
      // | ["typography-test", ..._] => <TypographyTest/>
      // | ["formation-test", ..._] => <FormationTest/>
      | _ => <Landing.Jsx3 dispatch title locale/>
    };
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

let forceSignInUrl: ReasonReact.Router.url = { path: ["sign-in"], hash: "", search: "" };


let routeSelector = (state: Store.state) => 
  Env.signInRequired && !(switch(state.state.user){ | SignedIn(_user) => true | _ => false })
    ? forceSignInUrl
    : state.state.state.route;

let localeSelector = (state: Store.state) => state.locale;

module Root = {
  [@react.component]
  let make = (~title: string) => {
    let route = AppStore.useSelector(routeSelector);
    let locale = AppStore.useSelector(localeSelector);

    <IntlProvider 
      locale=(locale|.Locale.toString)
      messages=(locale|.Locale.toMessages|.ReactIntl.messagesArrayToDict)>
        <ThemeProvider theme=AppTheme.theme> 
          <Shell state=route dispatch=AppStore.useDispatch() title locale/>
          <NotificationEpics.Context.Jsx3 dummy=""/>
        </ThemeProvider>
    </IntlProvider>
  };
};

[@react.component]
let make = () => 
  <AppStore.Provider store=Store.store>
    <Root title="tracereplay"/>
  </AppStore.Provider>