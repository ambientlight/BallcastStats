[@bs.config {jsx: 3}];

open Operators;

GlobalCss.inject();

// module RouterProvider = {
//   let forceSignInUrl: ReasonReact.Router.url = { path: ["sign-in"], hash: "", search: "" };
//   let make = Reductive.Lense.createMake(
//     ~lense=(state: ReductiveLocale.withLocale(ReductiveCognito.withAuth(ReductiveRouter.withRouter(State.t)))) => 
//       Env.signInRequired && !(switch(state.state.user){ | SignedIn(_user) => true | _ => false })
//         ? forceSignInUrl
//         : state.state.state.route,
//       Store.store);
// };

// module LocaleProvider = {
//   let make = Reductive.Lense.createMake(
//     ~lense=(state: ReductiveLocale.withLocale(ReductiveCognito.withAuth(ReductiveRouter.withRouter(State.t)))) => state.locale, 
//     Store.store);
// };

/** Base routing login goes here */
module Shell = {
  [@react.component]
  let make = (~state: ReasonReact.Router.url, ~dispatch, ~title, ~locale) => {
    Routes.({
      switch(state.path){
      | ["sign-in", ..._] => <Auth.Jsx3 title mode=SignIn/>
      | ["sign-up", ..._] => <Auth.Jsx3 title mode=SignUp/>
      | ["forgot", ..._] => <Auth.Jsx3 title mode=ForgotPassword/>
      | ["verify-sign-up", ..._] => <Auth.Jsx3 title mode=VerifySignUp/>
      // | ["typography-test", ..._] => <TypographyTest/>
      // | ["formation-test", ..._] => <FormationTest/>
      | _ => <Landing.Jsx3 dispatch title locale/>
      }
    })
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

module Inner = {
  [@react.component]
  let make = (~state as locale: Locale.locale, ~dispatch as _dispatch: Store.Action.t => unit, ~title: string) => {
    let route: ReasonReact.Router.url = {
      path: [],
      hash: "",
      search: ""
    };
    
    <IntlProvider 
      locale=(locale|.Locale.toString)
      messages=(locale|.Locale.toMessages|.ReactIntl.messagesArrayToDict)>
        <ThemeProvider theme=AppTheme.theme> 
          <Shell state=route dispatch=_dispatch title locale>
          </Shell>
          /* connect app shell with reductive router state */
          // <RouterProvider component=Shell.make(~title, ~locale)/>
          /* notification epics manages snackbars displayed based on actions dispatched */
          // <NotificationEpics.Context/>
        </ThemeProvider>
    </IntlProvider>
  };
};

module Root = {
  [@react.component]
  let make = () => 
    <Inner state=Locale.En dispatch=((_action) => ()) title="tracereplay"/>
};