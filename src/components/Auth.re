[@bs.config {jsx: 3}];

open Operators;
open Css;
open Webapi;
open AuthLocale;
open Utils.Tests;

type mode = SignIn | SignUp | ForgotPassword | VerifySignUp;
module Styles = AuthStyles;

type retained = {
  mutable formRef: option(Dom.HtmlElement.t),
  mutable emailRef: option(Dom.HtmlElement.t),
  mutable passwordRef: option(Dom.HtmlElement.t),
  willUnmount: Rx.Subject.t(bool)
};

type state = {
  email: string,
  password: string,
  passwordConfirmation: string,
  staySignedIn: bool,
  verificationCode: string,

  showsAutofillInSignIn: bool
};

/* actions to propagate to global store */
type actionsToPropagate = [
  | `SignInRequest(unit)
  | `SignUpRequest(unit)
  | `CompleteNewPasswordRequest(unit)
  | `RouterPushRoute(string)
  | `ForceVerificationRequired(string as 'code, string as 'username)
  | `ResendVerificationRequest(string as 'username)
  | `SignOutRequest(unit)
];

type action = [ 
  | `EmailChanged(string)
  | `PasswordChanged(string)
  | `PasswordConfirmationChanged(string)
  /* used on return key to send */
  | `SubmitVerificationCode(string as 'username)
  | `VerificationCodeChanged(string as 'code, string as 'username)
  | `StaySignedInChanged(bool)
  | `DevToolStateUpdate(state)
  | `ShowsAutofillInSignIn(bool)
  | actionsToPropagate
];

module Forms {
  let signIn = (state, dispatch: action => unit) =>
    <form className=Styles.form /*ref=(element => { retained.formRef = Js.Nullable.toOption(!!element) })*/ id="AuthSignInForm">
      <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
        <DefinedMessage message=(Env.signInRequired ? strings##beenHereBefore : strings##gladToSeeYouBack)/>
      </MaterialUi.Typography>
      <MaterialUi.TextField value=`String(state.email)
        className=(Styles.textField|.withTestClass("test-sign-in-email-field"))
        autoComplete="username"
        type_="email"
        label=<DefinedMessage message=CommonLocale.strings##email/>
        // inputRef=`Callback((element: Js.Nullable.t(Dom.HtmlElement.t)) => {
        //   retained.emailRef = Js.Nullable.toOption(element);
        // })
        onChange=(event => dispatch(`EmailChanged(ReactEvent.Form.target(event)##value)))/>

      <MaterialUi.TextField value=`String(state.password)
        className=(Styles.textField|.withTestClass("test-sign-in-password-field"))
        autoComplete="current-password"
        type_="password"
        label=<DefinedMessage message=CommonLocale.strings##password/>
        // inputRef=`Callback((element: Js.Nullable.t(Dom.HtmlElement.t)) => Rx.Observable.Operators.({
        //   Rx.Observable.fromEvent(element, "keydown")
        //   |> takeUntil(retained.willUnmount |. Rx.Subject.asObservable)
        //   |> filter((event: ReactEvent.Keyboard.t) => ReactEvent.Keyboard.keyCode(event) == 13)
        //   |> Rx.Observable.subscribe(~next=(_event => dispatch(`SignInRequest())))
        // }))
        onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))/>

      <div className=Styles.actionPanel>
        <MaterialUi.FormControl>
          <MaterialUi.FormControlLabel 
            label=<DefinedMessage message=CommonLocale.strings##staySignedIn/>
            control=<MaterialUi.Checkbox color=`Default
              value="remember"
              checked=state.staySignedIn
              // onChange=((_event, value) => dispatch(`StaySignedInChanged(value)))
            />
          />
        </MaterialUi.FormControl>
        // (!Env.signInRequired 
        //   ? <LabelButton color=`TextPrimary onClick=(_event => dispatch(`RouterPushRoute(Routes.forgot)))>
        //       <MaterialUi.Typography variant=`Body2 color=`Inherit>
        //         <ReactIntl.DefinedMessage message=CommonLocale.strings##forgotYourPassword/>
        //       </MaterialUi.Typography>
        //     </LabelButton>
        //   : <Fragment/>
        // )
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
      
      (!Env.signInRequired
        ? <div className=Styles.signUpContainer>
            <MaterialUi.Typography variant=`Subtitle1 style=ReactDOMRe.Style.make(~display="inline", ())>
              <DefinedMessage message=strings##dontHaveAnAccount/>
            </MaterialUi.Typography>
            // <LabelButton color=`TextPrimary 
            //   className="test-sign-in-alt-signup-button"
            //   emphasized=true 
            //   onClick=(_event => dispatch(`RouterPushRoute(Routes.signUp)))>
            //   <MaterialUi.Typography style=ReactDOMRe.Style.make(~display="inline", ())>
            //     <ReactIntl.DefinedMessage message=CommonLocale.strings##signUp/>
            //   </MaterialUi.Typography>
            // </LabelButton>
          </div>
        : <div></div>
      )
    </form>;
  
  let signUp = (_state, dispatch: action => unit) =>
    <form className=Styles.form id="AuthSignUpForm">
      <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
        <DefinedMessage message=strings##createYourAccount/>
      </MaterialUi.Typography>
      <MaterialUi.TextField
        className=(Styles.textField|.withTestClass("test-sign-up-username-field"))
        /* 
          * FIXME: https://github.com/ambientlight/BallcastStats/issues/6
          * using username for some reason results in inability to change input inside unit test 
          */
        /* type_="username" */
        autoComplete="username"
        label=<DefinedMessage message=CommonLocale.strings##email/>
        onChange=(event => dispatch(`EmailChanged(ReactEvent.Form.target(event)##value)))/>
      <MaterialUi.TextField
        className=(Styles.textField|.withTestClass("test-sign-up-password-field"))
        type_="password"
        autoComplete="new-password"
        label=<DefinedMessage message=CommonLocale.strings##password/>
        onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))/>
      <MaterialUi.TextField
        className=(Styles.textField|.withTestClass("test-sign-up-confirmpassword-field"))
        type_="password"
        autoComplete="new-password"
        label=<DefinedMessage message=CommonLocale.strings##confirmPassword/>
        // inputRef=`Callback((element: Js.Nullable.t(Dom.HtmlElement.t)) => Rx.Observable.Operators.({
        //   Rx.Observable.fromEvent(element, "keydown")
        //   |> takeUntil(retained.willUnmount |. Rx.Subject.asObservable)
        //   |> filter((event: ReactEvent.Keyboard.t) => ReactEvent.Keyboard.keyCode(event) == 13)
        //   |> Rx.Observable.subscribe(~next=(_event => dispatch(`SignUpRequest())))
        // }))
        onChange=(event => dispatch(`PasswordConfirmationChanged(ReactEvent.Form.target(event)##value)))/>

      <MaterialUi.Button variant=`Outlined
        className=(Styles.button|.withTestClass("test-sign-up-button"))
        onClick=(_event => dispatch(`SignUpRequest(())))>
        <DefinedMessage message=CommonLocale.strings##signUp/>
      </MaterialUi.Button>
      <div className=Styles.signUpContainer>
        <MaterialUi.Typography variant=`Subtitle1 style=ReactDOMRe.Style.make(~display="inline", ())>
          <DefinedMessage message=strings##alreadyHaveAnAccount/>
        </MaterialUi.Typography>
        // <LabelButton color=`TextPrimary
        //   className="test-sign-up-alt-signin-button"
        //   emphasized=true 
        //   onClick=(_event => dispatch(`RouterPushRoute(Routes.signIn)))>
        //   <MaterialUi.Typography style=ReactDOMRe.Style.make(~display="inline", ())>
        //     <ReactIntl.DefinedMessage message=CommonLocale.strings##signIn/>
        //   </MaterialUi.Typography>
        // </LabelButton>
      </div>
      /* <div className=Styles.additionalInfoContainer>
        <MaterialUi.Typography variant=`Caption style=ReactDOMRe.Style.make(~display="inline", ())>
          <ReactIntl.DefinedMessage message=strings##termsOfUseP1/>
        </MaterialUi.Typography>
        <LabelButton color=`TextPrimary emphasized=true>
          <MaterialUi.Typography variant=`Caption style=ReactDOMRe.Style.make(~display="inline", ())>
            <ReactIntl.DefinedMessage message=strings##termsOfUseP2/>
          </MaterialUi.Typography>
        </LabelButton>
        <MaterialUi.Typography variant=`Caption style=ReactDOMRe.Style.make(~display="inline", ())>
          <ReactIntl.DefinedMessage message=strings##termsOfUseP3/>
        </MaterialUi.Typography>
        <LabelButton color=`TextPrimary emphasized=true>
          <MaterialUi.Typography variant=`Caption style=ReactDOMRe.Style.make(~display="inline", ())>
            <ReactIntl.DefinedMessage message=strings##termsOfUseP4/>
          </MaterialUi.Typography>
        </LabelButton>
      </div> */
    </form>;

  let forgotPassword = (_state, _dispatch: action => unit) =>
    <form className=Styles.form id="AuthForgotPasswordForm">
      <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
        <DefinedMessage message=CommonLocale.strings##forgotYourPassword/>
      </MaterialUi.Typography>
      <MaterialUi.Typography variant=`Subtitle1 className=([Styles.smallTopMargin, Styles.forgotPasswordDescription]>|<" ")>
        <DefinedMessage message=strings##forgotPasswordDescription/>
      </MaterialUi.Typography>
      <MaterialUi.TextField
        type_="email"
        label=<DefinedMessage message=CommonLocale.strings##email/>
        className=Styles.textField/>
      <MaterialUi.Button variant=`Outlined className=Styles.button>
        <DefinedMessage message=CommonLocale.strings##send/>
      </MaterialUi.Button>
    </form>;

  let newPassword = (state, dispatch: action => unit) =>
    <form className=Styles.form autoComplete="nope">
      <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
        <DefinedMessage message=strings##newPasswordRequired/>
      </MaterialUi.Typography>
      <MaterialUi.Typography variant=`Subtitle1 className=Styles.smallTopMargin>
        <DefinedMessage message=strings##newPasswordRequiredDescription/>
      </MaterialUi.Typography>
      <MaterialUi.TextField value=`String(state.password)
        className=Styles.textField
        name="password"
        type_="password"
        autoComplete="nope"
        label=<DefinedMessage message=CommonLocale.strings##password/>
        onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))
        />
      <MaterialUi.TextField value=`String(state.passwordConfirmation)
        className=Styles.textField
        type_="password"
        autoComplete="nope"
        label=<DefinedMessage message=CommonLocale.strings##confirmPassword/>
        onChange=(event => dispatch(`PasswordConfirmationChanged(ReactEvent.Form.target(event)##value)))
        />
      <MaterialUi.Button variant=`Outlined
        className=Styles.button 
        onClick=(_event => dispatch(`CompleteNewPasswordRequest()))>
        <DefinedMessage message=CommonLocale.strings##confirm/>
      </MaterialUi.Button>
    </form>;

  let accountVerification = (~username, ~signInState: ReductiveCognito.signInState, ~state, ~dispatch: action => unit) => {
    let verifying = switch(signInState){ | Verifying(_code, _username) => true | _ => false };
    let codeIncorrect = switch(signInState){ | AccountVerificationError(error, _code, _username) => error|.Amplify.Error.codeGet == "CodeMismatchException" | _ => false };
    let codeExpired = switch(signInState){ | AccountVerificationError(error, _, _) => error|.Amplify.Error.codeGet == "ExpiredCodeException" | _ => false };
    let resendingCode = switch(signInState){ | ResendingVerification(_) => true | _ => false };

    <form 
      className=Styles.form autoComplete="nope"
      id="AuthVerifySignInForm"
      // ref=(element => {
      //   let _ = (element|.Js.Nullable.toOption)
      //   |. Belt.Option.map(element => {
      //     let focusIdx = String.length(state.verificationCode);
      //     let inputs = !!element
      //       |> ElementRe.querySelectorAll("input")
      //       |. NodeListRe.toArray;
      //     /* support return key to send */
      //     inputs |. Belt.Array.forEach(input => Rx.Observable.Operators.({
      //       let _ = Rx.Observable.fromEvent(input, "keydown")
      //         |> takeUntil(retained.willUnmount |. Rx.Subject.asObservable)
      //         |> filter((event: ReactEvent.Keyboard.t) => ReactEvent.Keyboard.keyCode(event) == 13)
      //         |> Rx.Observable.subscribe(~next=_event => {
      //           dispatch(`SubmitVerificationCode(username))
      //         });
      //     }));
      //     /* 
      //       (for verification code reset) apply only when verification code is 1 digit
      //       FIXME: https://github.com/ambientlight/BallcastStats/issues/4 
      //       this is still hacky since user cannot errase by backspace completely
      //     */
      //     if(String.length(state.verificationCode) == 1 && focusIdx < Array.length(inputs)){
      //       Dom.HtmlElement.focus(!!(inputs[focusIdx]));
      //     }
      //   });
      // })
      >

      <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
        <DefinedMessage message=strings##verifyYourAccount/>
      </MaterialUi.Typography>
      <MaterialUi.Typography variant=`Subtitle1>
        <DefinedMessage message=strings##verifyYourAccountDetail/>
      </MaterialUi.Typography>
      <MaterialUi.Typography variant=`Subtitle1 className=Styles.smallTopMargin>
        <DefinedMessage message=strings##pleaseEnterItHere/>
      </MaterialUi.Typography>
      // <ReactCodeInput
      //   disabled=(verifying || codeExpired)
      //   value=state.verificationCode
      //   className=([Styles.codeInputBase, codeIncorrect || codeExpired ? Styles.errorCodeInput : Styles.normalCodeInput, verifying ? Styles.disabledCodeInput : ""] >|< " ")
      //   type_="number"
      //   fields=6
      //   onChange=(event => String.({
      //     /*
      //       * a bit hacky
      //       * since ReactCodeInput doesn't have a callback with each letter
      //       * on error we calculate a diff to find a changed letter and technically reset the input to it
      //       * but since FIXME: https://github.com/ambientlight/BallcastStats/issues/4 ReactCodeInput doesn't YET support two way binding (only initial value)
      //       * we trigger the reductive action
      //       * which on signIn state change will result in full reinit of this component (because of component spread hack below)
      //       */
      //     if(codeIncorrect || codeExpired && state.verificationCode|.length == 6){
      //       let idxs = Array.init(min(event|.length, state.verificationCode|.length), x => x) |> Array.to_list;
      //       switch(
      //         idxs 
      //         |> List.find(idx => (event|.get(idx)) != (state.verificationCode|.get(idx)))){
      //       | exception Not_found => dispatch(`ForceVerificationRequired(make(1, event|.get(0)), username))
      //       | result => dispatch(`ForceVerificationRequired(make(1, event|.get(result)), username))
      //       }        
      //     } else {
      //       dispatch(`VerificationCodeChanged(event, username))
      //     }
      //   }))/>
      <MaterialUi.Typography color=`Error variant=`Subtitle1 style=ReactDOMRe.Style.make(~height="24px", ())>
        {
          codeExpired ? <DefinedMessage message=strings##codeExpiredPleaseResend/> :
          codeIncorrect ? <DefinedMessage message=strings##verificationCodeIncorrect/> : ReasonReact.string("")
        }
      </MaterialUi.Typography>

      {verifying || resendingCode
        ? <MaterialUi.Button variant=`Outlined
            disabled=true
            className=merge([Styles.button, style([opacity(0.7)])])
            onClick=(_event => ())>
            {resendingCode 
              ? <DefinedMessage message=strings##resendingThreeDots/>
              : <DefinedMessage message=strings##verifyingThreeDots/>
            }
          </MaterialUi.Button>
        : <MaterialUi.Button variant=`Outlined 
            className=Styles.button
            onClick=(_event => dispatch(`ResendVerificationRequest(username)))>
            <DefinedMessage message=strings##resendVerificationCode/>
          </MaterialUi.Button>
      }
    </form>
  };
}

let didAutofillObservable = retained => Rx.Observable.Operators.(
  Rx.Observable.intervalFromScheduler(Rx.Scheduler.animationFrame)
  |> take(500)
  |> map(_value => 
    switch(retained.formRef){
    | Some(formRef) => 
      !!formRef
      |> ElementRe.querySelectorAll("input:-webkit-autofill")
      |. NodeListRe.toArray
      |> Array.fold_left((target, node) => {
        target
        || Belt.Option.eq(Some(node), retained.emailRef, (lhs, rhs) => !!lhs === rhs)
        || Belt.Option.eq(Some(node), retained.passwordRef, (lhs, rhs) => !!lhs === rhs)
      }, false)
    | None => false
    })
  |> filter(value => value)
  |> take(1));
    

[@react.component]
let make = (~state as signInState: ReductiveCognito.signInState, ~dispatch, ~mode: mode, ~title) => {
  /* 
    * FIXME: Intentionally making component recreate on each prop change for reactCodeInput code to reset on incorrect code 
    * until https://github.com/ambientlight/BallcastStats/issues/4 lands
    */
  let (state, send) = React.useReducer(
    (state, action) =>
    switch (action) {
    | `EmailChanged(email) => ({ ...state, email, showsAutofillInSignIn: false })
    | `PasswordChanged(password) => ({ ...state, password, showsAutofillInSignIn: false })
    | `PasswordConfirmationChanged(passwordConfirmation) => ({ ...state, passwordConfirmation })
    | `DevToolStateUpdate(devToolsState) => (devToolsState)
    | `StaySignedInChanged(staySignedIn) => ({ ...state, staySignedIn })
    // | `SubmitVerificationCode(username) => 
    //   ReasonReact.SideEffects(_self => {
    //     String.length(state.verificationCode) == 6 
    //     ? dispatch(`ConfirmSignUpRequest(state.verificationCode, username))
    //     : ()
    //   })
    // | `VerificationCodeChanged(verificationCode, username) => 
    //   String.length(verificationCode) == 6 
    //     ? WithSideEffects(
    //       { ...state, verificationCode }, 
    //       _self => dispatch(`ConfirmSignUpRequest(verificationCode, username)))
    //     : ({ ...state, verificationCode })
    | `ShowsAutofillInSignIn(showsAutofillInSignIn) => ({ ...state, showsAutofillInSignIn })
    
    // | `ForceVerificationRequired(code, username) => ReasonReact.SideEffects(_self => dispatch(`ForceVerificationRequired(code, username))) 
    // | `RouterPushRoute(route) => ReasonReact.SideEffects(_self => dispatch(`RouterPushRoute(route)))
    // | `CompleteNewPasswordRequest() => ReasonReact.SideEffects(_self => dispatch(`CompleteNewPasswordRequest(state.password)))
    // | `ResendVerificationRequest(username) => ReasonReact.SideEffects(_self => dispatch(`ResendVerificationRequest(username)))
    // | `SignOutRequest() => ReasonReact.SideEffects(_self => dispatch(`SignOutRequest(())))
    // | `SignInRequest() => ReasonReact.SideEffects(_self => dispatch(`SignInRequest(state.email, state.password)))
    // | `SignUpRequest() => ReasonReact.SideEffects(_self => 
    //     state.password == state.passwordConfirmation 
    //       ? dispatch(`SignUpRequest(state.email, state.password))
    //       : dispatch(`SignUpRequestRejected(Amplify.Error.t(~code="PasswordAndConfirmationDoNotMatchException", ~name="PasswordAndConfirmationDoNotMatchException", ~message="Password and confirmation do not match")))) 
    },
    {
      email: "",
      password: "",
      passwordConfirmation: "",
      staySignedIn: false,
      verificationCode: 
        switch(signInState){
        | AccountVerificationRequired(code, _)
        | Verifying(code, _)
        | AccountVerificationError(_, code, _) => code
        | _ => ""
        },

      showsAutofillInSignIn: false
    }
  );


  // didMount: self => Rx.Observable.Operators.({
  //   let _sub = didAutofillObservable(self.retainedProps)
  //   |> filter(value => value)
  //   |> takeUntil(self.retainedProps.willUnmount |. Rx.Subject.asObservable)
  //   |> Rx.Observable.subscribe(~next=_value => self.send(`ShowsAutofillInSignIn(true)));

  //   ReductiveDevTools.Connectors.register(
  //     ~connectionId=__MODULE__,
  //     ~component=self,
  //     ~options=ReductiveDevTools.Extension.enhancerOptions(()),
  //     ()
  //   )
  // }),

  // willUnmount: self => Rx.Subject.next(self.retainedProps.willUnmount, true),

  //reducer: ReductiveDevTools.Connectors.componentReducerEnhancer(__MODULE__) @@,

  <div className=Styles.root>
    <Logo.WithCaption 
      dispatch
      className=([Styles.logo, Styles.hideLogoHackOnNarrowLayout] >|< " ") 
      caption=title
      hideCaptionOnSmall=false/>
    
    <MaterialUi.Card className=Styles.card>
      {switch((mode, signInState)){
      | (_, SigningIn()) => <MaterialUi.CircularProgress size=`Int(128) className=Styles.progressSpinner/>
      | (SignIn, SignedIn(user)) when (user |. Amplify.Auth.CognitoUser.challengeNameGet) == Some("NEW_PASSWORD_REQUIRED") => 
        Forms.newPassword(state, !!send)
      | (VerifySignUp, AccountVerificationRequired(_, username))
      | (VerifySignUp, Verifying(_, username))
      | (VerifySignUp, ResendingVerification(username))
      | (VerifySignUp, AccountVerificationError(_, _, username)) => Forms.accountVerification(~username, ~signInState, ~state, ~dispatch=(!!send))
      | (VerifySignUp, _) => 
        <div id="AuthVerifySignInForm">
          <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
            <DefinedMessage message=strings##verificationIsNotNeeded/>
          </MaterialUi.Typography>
        </div>
      | (SignIn, SignedIn(_user)) => 
        <div>
          <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
            <DefinedMessage message=strings##youAreSignedIn/>
          </MaterialUi.Typography>
          <MaterialUi.Button variant=`Outlined 
            className=Styles.button
            // onClick=(_event => send(`SignOutRequest(())))
            >
            <DefinedMessage message=CommonLocale.strings##logout/>
          </MaterialUi.Button>
        </div>
      | (SignIn, _) => Forms.signIn(state, !!send)
      | (SignUp, _) => Forms.signUp(state, !!send)
      | (ForgotPassword, _) => Forms.forgotPassword(state, !!send)
      }}
    </MaterialUi.Card>
  </div>
};