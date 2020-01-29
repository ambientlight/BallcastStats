open Webapi;
open AuthDefs;
open Operators;
open Shortener;
open AuthLocale;
open Utils.Tests;

module Styles = AuthStyles;

[@react.component]
let make = (~state, ~retained: retained, ~dispatch: action => unit) => {
  let confirmPasswordRef: React.Ref.t(Js.Nullable.t(Dom.Element.t)) = React.useRef(Js.Nullable.null);

  /**
    adds support for 'Hit Return to send' in confirm password input
   */
  React.useEffect0(() => {
    confirmPasswordRef |> refc |. toopt
    |. optfmap(element => element |> Webapi.Dom.Element.querySelector("input"))
    |. optmap(element => Rx.Operators.({
      Rx.fromEvent(~target=element, ~eventName="keydown")
      |> takeUntil(retained.willUnmount |. Rx.Subject.asObservable)
      |> filter((event: ReactEvent.Keyboard.t, _idx) => ReactEvent.Keyboard.keyCode(event) == 13)
      |> Rx.Observable.subscribe(~next=(_event => dispatch(`SignUpRequest())))
      |> ignore
      element 
    })) 
    |> ignore;

    None
  });

  <form className=Styles.form id="AuthSignUpForm">
    <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
      <DefinedMessage message=strings##createYourAccount/>
    </MaterialUi.Typography>

    <MaterialUi.TextField value=`String(state.email)
      className=(Styles.textField|.withTestClass("test-sign-up-username-field"))
      /* 
        * FIXME: https://github.com/ambientlight/BallcastStats/issues/6
        * using username for some reason results in inability to change input inside unit test 
        */
      /* type_="username" */
      autoComplete="username"
      label=<DefinedMessage message=CommonLocale.strings##email/>
      onChange=(event => dispatch(`EmailChanged(ReactEvent.Form.target(event)##value)))/>

    <MaterialUi.TextField value=`String(state.password)
      className=(Styles.textField|.withTestClass("test-sign-up-password-field"))
      type_="password"
      autoComplete="new-password"
      label=<DefinedMessage message=CommonLocale.strings##password/>
      onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))/>

    <MaterialUi.TextField value=`String(state.passwordConfirmation)
      className=(Styles.textField|.withTestClass("test-sign-up-confirmpassword-field"))
      type_="password"
      autoComplete="new-password"
      label=<DefinedMessage message=CommonLocale.strings##confirmPassword/>
      ref={ReactDOMRe.Ref.domRef(confirmPasswordRef)}
      onChange=(event => dispatch(`PasswordConfirmationChanged(ReactEvent.Form.target(event)##value)))/>

    <MaterialUi.Button variant=`Outlined
      className=(Styles.button|.withTestClass("test-sign-up-button"))
      onClick=(_event => dispatch(`SignUpRequest(())))>
      <DefinedMessage message=CommonLocale.strings##signUp/>
    </MaterialUi.Button>

    /** link to sign in */
    <div className=Styles.signUpContainer>
      <MaterialUi.Typography variant=`Subtitle1 style=ReactDOMRe.Style.make(~display="inline", ())>
        <DefinedMessage message=strings##alreadyHaveAnAccount/>
      </MaterialUi.Typography>
      <LabelButton color=`TextPrimary
        className="test-sign-up-alt-signin-button"
        emphasized=true 
        onClick=(_event => dispatch(`RouterPushRoute(Routes.signIn)))>
        <MaterialUi.Typography style=ReactDOMRe.Style.make(~display="inline", ())>
          <DefinedMessage message=CommonLocale.strings##signIn/>
        </MaterialUi.Typography>
      </LabelButton>
    </div>

    /** by registring you agree to .... */
    <div className=Styles.additionalInfoContainer>
      <MaterialUi.Typography variant=`Caption style=ReactDOMRe.Style.make(~display="inline", ())>
        <DefinedMessage message=strings##termsOfUseP1/>
      </MaterialUi.Typography>
      <LabelButton color=`TextPrimary emphasized=true>
        <MaterialUi.Typography variant=`Caption style=ReactDOMRe.Style.make(~display="inline", ())>
          <DefinedMessage message=strings##termsOfUseP2/>
        </MaterialUi.Typography>
      </LabelButton>
      <MaterialUi.Typography variant=`Caption style=ReactDOMRe.Style.make(~display="inline", ())>
        <DefinedMessage message=strings##termsOfUseP3/>
      </MaterialUi.Typography>
      <LabelButton color=`TextPrimary emphasized=true>
        <MaterialUi.Typography variant=`Caption style=ReactDOMRe.Style.make(~display="inline", ())>
          <DefinedMessage message=strings##termsOfUseP4/>
        </MaterialUi.Typography>
      </LabelButton>
    </div>
  </form>;
}