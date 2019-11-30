open Shortener;
open Operators;
open Webapi;
open AuthDefs;
open AuthLocale;
open Utils.Tests;

module Styles = AuthStyles;

[@react.component]
let make = (~state, ~retained: retained, ~dispatch: action => unit) => {
  let emailRef: React.Ref.t(Js.Nullable.t(Dom.Element.t)) = React.useRef(Js.Nullable.null);
  let passwordRef: React.Ref.t(Js.Nullable.t(Dom.Element.t)) = React.useRef(Js.Nullable.null);
  let formRef: React.Ref.t(Js.Nullable.t(Dom.Element.t)) = React.useRef(Js.Nullable.null);

  /**
    1. saves form/email/password element references, so that root component can check if autofill was applied
    2. adds support for 'Hit Return to send' in password input
   */
  React.useEffect(() => {
    retained.formRef = formRef |> refc |. toopt;

    retained.emailRef = emailRef |> refc |. toopt
    |. optfmap(element => element |> Webapi.Dom.Element.querySelector("input"));

    retained.passwordRef = passwordRef |> refc |. toopt
    |. optfmap(element => element |> Webapi.Dom.Element.querySelector("input"))
    |. optmap(element => Rx.Observable.Operators.({
      Rx.Observable.fromEvent(element, "keydown")
      |> takeUntil(retained.willUnmount |. Rx.Subject.asObservable)
      |> filter((event: ReactEvent.Keyboard.t) => ReactEvent.Keyboard.keyCode(event) == 13)
      |> tap(~next=event => ~~event)
      |> Rx.Observable.subscribe(~next=(_event => dispatch(`SignInRequest())))
      |> ignore;
      element 
    }));

    None
  });

  <form className=Styles.form ref={ReactDOMRe.Ref.domRef(formRef)} id="AuthSignInForm">
    <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
      <DefinedMessage message=(Env.signInRequired ? strings##beenHereBefore : strings##gladToSeeYouBack)/>
    </MaterialUi.Typography>
    
    <MaterialUi.TextField value=`String(state.email)
      className=(Styles.textField|.withTestClass("test-sign-in-email-field"))
      autoComplete="username"
      type_="email"
      label=<DefinedMessage message=CommonLocale.strings##email/>
      ref={ReactDOMRe.Ref.domRef(emailRef)}
      onChange=(event => dispatch(`EmailChanged(ReactEvent.Form.target(event)##value)))/>

    <MaterialUi.TextField value=`String(state.password)
      className=(Styles.textField|.withTestClass("test-sign-in-password-field"))
      autoComplete="current-password"
      type_="password"
      label=<DefinedMessage message=CommonLocale.strings##password/>
      ref={ReactDOMRe.Ref.domRef(passwordRef)}
      onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))/>

    <div className=Styles.actionPanel>
      <MaterialUi.FormControl>
        <MaterialUi.FormControlLabel 
          label=<DefinedMessage message=CommonLocale.strings##staySignedIn/>
          control=<MaterialUi.Checkbox color=`Default
            value="remember"
            checked=state.staySignedIn
            onChange=(event => dispatch(`StaySignedInChanged(ReactEvent.Form.target(event)##checked)))
          />
        />
      </MaterialUi.FormControl>
      (!Env.signInRequired 
        ? <LabelButton color=`TextPrimary onClick=(_event => dispatch(`RouterPushRoute(Routes.forgot)))>
            <MaterialUi.Typography variant=`Body2 color=`Inherit>
              <DefinedMessage message=CommonLocale.strings##forgotYourPassword/>
            </MaterialUi.Typography>
          </LabelButton>
        : (<>{React.string("")}</>)
      )
    </div>

    <MaterialUi.Button variant=`Outlined
      className=(Styles.button|.withTestClass("test-sign-in-button"))
      onClick=(_event => { 
        dispatch(`SignInRequest());
      })>
      {state.showsAutofillInSignIn 
        ? <DefinedMessage message=strings##useAutofilledCredentials/>
        : <DefinedMessage message=CommonLocale.strings##signIn/>
      }
    </MaterialUi.Button>
    <MaterialUi.Button variant=`Outlined
      className=(Styles.button|.withTestClass("test-sign-in-button"))
      onClick=(_event => { 
        dispatch(`Increment(()));
      })>
      {state.accumulator}
    </MaterialUi.Button>
    
    /** hides link to sign up in a restricted mode */
    (!Env.signInRequired
      ? <div className=Styles.signUpContainer>
          <MaterialUi.Typography variant=`Subtitle1 style=ReactDOMRe.Style.make(~display="inline", ())>
            <DefinedMessage message=strings##dontHaveAnAccount/>
          </MaterialUi.Typography>
          <LabelButton color=`TextPrimary 
            className="test-sign-in-alt-signup-button"
            emphasized=true 
            onClick=(_event => dispatch(`RouterPushRoute(Routes.signUp)))>
            <MaterialUi.Typography style=ReactDOMRe.Style.make(~display="inline", ())>
              <DefinedMessage message=CommonLocale.strings##signUp/>
            </MaterialUi.Typography>
          </LabelButton>
        </div>
      : (<>{React.string("")}</>)
    )
  </form>;
}