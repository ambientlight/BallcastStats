open Operators;
open Css;
open Webapi;

type mode = SignIn | SignUp | ForgotPassword | VerifySignUp;
module Styles = AuthStyles;

module Inner {
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
    let signIn = (state, retained, dispatch: action => unit) =>
      <form className=Styles.form ref=(element => { retained.formRef = Js.Nullable.toOption(!!element) })>
        <span className=Styles.welcomeTitle>{ReasonReact.string("Glad to see you back!")}</span>
        <MaterialUi.TextField value=`String(state.email)
          /* FIXME: when using wrapped TextField focus gets messed up, in the meantime use the wrapped styles here directly */
          className=([TextField.Styles.baseMargin, Styles.textField] >|< " ")
          _InputLabelProps=TextField.Styles.inputLabelProps
          _InputProps=TextField.Styles.inputProps

          inputRef=`Callback((element: Js.Nullable.t(Dom.HtmlElement.t)) => {
            retained.emailRef = Js.Nullable.toOption(element);
          })

          autoComplete="username"
          type_="email"
          label=ReasonReact.string("email")
          onChange=(event => dispatch(`EmailChanged(ReactEvent.Form.target(event)##value)))/>

        <MaterialUi.TextField value=`String(state.password)
          /* FIXME: when using wrapped TextField focus gets messed up, in the meantime use the wrapped styles here directly */
          className=([TextField.Styles.baseMargin, Styles.textField] >|< " ")
          _InputLabelProps=TextField.Styles.inputLabelProps
          _InputProps=TextField.Styles.inputProps

          inputRef=`Callback((element: Js.Nullable.t(Dom.HtmlElement.t)) => Rx.Observable.Operators.({
            Rx.Observable.fromEvent(element, "keydown")
            |> takeUntil(retained.willUnmount |. Rx.Subject.asObservable)
            |> filter((event: ReactEvent.Keyboard.t) => ReactEvent.Keyboard.keyCode(event) == 13)
            |> Rx.Observable.subscribe(~next=(_event => dispatch(`SignInRequest())))
          }))
          autoComplete="current-password"
          type_="password"
          label=ReasonReact.string("password")
          onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))/>

        <div className=Styles.actionPanel>
          <MaterialUi.FormControl>
            <MaterialUi.FormControlLabel 
              label=<span className=Styles.label>{ReasonReact.string("Stay signed in")}</span>
              control=<MaterialUi.Checkbox
                value="remember"
                checked=`Bool(state.staySignedIn)
                onChange=((_event, value) => dispatch(`StaySignedInChanged(value)))
                classes=[
                  MaterialUi.Checkbox.Classes.Root(Styles.checkbox),
                  MaterialUi.Checkbox.Classes.Checked(Styles.checkbox)
                ]/>
            />
          </MaterialUi.FormControl>
          <span
            className=([Styles.accesoryLabel, Styles.actionLabel] >|< " ")
            onClick=(_event => dispatch(`RouterPushRoute(Routes.forgot)))>
            {ReasonReact.string("Forgot password?")}
            </span>
        </div>
        <Button.Blended 
          className=Styles.button 
          onClick=(_event => { 
            dispatch(`SignInRequest());
          })>
          {state.showsAutofillInSignIn ? "Use Autofilled Credentials" : "Sign In"}
        </Button.Blended>
        
        <div className=Styles.signUpContainer>
          <span className=Styles.accesoryLabel>{ReasonReact.string("Don't have an account?")}</span>
          <span 
            className=([Styles.label, Styles.actionLabel] >|< " ")
            onClick=(_event => dispatch(`RouterPushRoute(Routes.signUp)))>
            {ReasonReact.string("Sign up")}
          </span>
        </div>
      </form>;
    
    let signUp = (state, retained, dispatch: action => unit) =>
      <form className=Styles.form>
        <span className=Styles.welcomeTitle>{ReasonReact.string("Create your account")}</span>
        <MaterialUi.TextField
          /* FIXME: when using wrapped TextField focus gets messed up, in the meantime use the wrapped styles here directly */
          className=([TextField.Styles.baseMargin, Styles.textField] >|< " ")
          _InputLabelProps=TextField.Styles.inputLabelProps
          _InputProps=TextField.Styles.inputProps

          type_="username"
          autoComplete="username"
          label=ReasonReact.string("email")
          onChange=(event => dispatch(`EmailChanged(ReactEvent.Form.target(event)##value)))/>
        <MaterialUi.TextField
          /* FIXME: when using wrapped TextField focus gets messed up, in the meantime use the wrapped styles here directly */
          className=([TextField.Styles.baseMargin, Styles.textField] >|< " ")
          _InputLabelProps=TextField.Styles.inputLabelProps
          _InputProps=TextField.Styles.inputProps

          type_="password"
          autoComplete="new-password"
          label=ReasonReact.string("password")
          onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))/>
        <MaterialUi.TextField
          /* FIXME: when using wrapped TextField focus gets messed up, in the meantime use the wrapped styles here directly */
          className=([TextField.Styles.baseMargin, Styles.textField] >|< " ")
          _InputLabelProps=TextField.Styles.inputLabelProps
          _InputProps=TextField.Styles.inputProps

          inputRef=`Callback((element: Js.Nullable.t(Dom.HtmlElement.t)) => Rx.Observable.Operators.({
            Rx.Observable.fromEvent(element, "keydown")
            |> takeUntil(retained.willUnmount |. Rx.Subject.asObservable)
            |> filter((event: ReactEvent.Keyboard.t) => ReactEvent.Keyboard.keyCode(event) == 13)
            |> Rx.Observable.subscribe(~next=(_event => dispatch(`SignUpRequest())))
          }))
          
          type_="password"
          autoComplete="new-password"
          label=ReasonReact.string("confirm password")
          onChange=(event => dispatch(`PasswordConfirmationChanged(ReactEvent.Form.target(event)##value)))/>
        <Button.Blended 
          className=Styles.button
          onClick=(_event => dispatch(`SignUpRequest(())))>
            "Sign Up"
        </Button.Blended>
        <div className=Styles.signUpContainer>
          <span className=Styles.accesoryLabel>{ReasonReact.string("Already have an account?")}</span>
          <span 
            className=([Styles.label, Styles.actionLabel] >|< " ")
            onClick=(_event => dispatch(`RouterPushRoute(Routes.signIn)))>
            {ReasonReact.string("Sign in")}
          </span>
        </div>
        <div className=Styles.additionalInfoContainer>
          <span className=Styles.additionalInfoLabel>{ReasonReact.string("By registering, you agree to Ballcast Stat's")}</span>
          <span className=([Styles.additionalInfoLabel, Styles.actionLabel] >|< " ")>{ReasonReact.string("Terms of Service")}</span>
          <span className=Styles.additionalInfoLabel>{ReasonReact.string("and")}</span>
          <span className=([Styles.additionalInfoLabel, Styles.actionLabel] >|< " ")>{ReasonReact.string("Privacy Policy.")}</span>
        </div>
      </form>;

    let forgotPassword = (state, dispatch: action => unit) =>
      <form className=Styles.form>
        <span className=Styles.welcomeTitle>{ReasonReact.string("Forgot your password?")}</span>
        <span className=merge([Styles.accesoryLabel, Styles.smallTopMargin])>{ReasonReact.string("Don't worry, please enter your email address, if there an account asssociated with it, we will send the reset password email to it shortly.")}</span>
        <TextField
          type_="email"
          label=ReasonReact.string("email")
          className=Styles.textField/>
        <Button.Blended className=Styles.button>"Send"</Button.Blended>
      </form>;

    let newPassword = (state, dispatch: action => unit) =>
      <form className=Styles.form autoComplete="nope">
        <span className=Styles.welcomeTitle>{ReasonReact.string("New password required")}</span>
        <span className=merge([Styles.accesoryLabel, Styles.smallTopMargin])>{ReasonReact.string("You have logged in with a temporary password, you new to create a new password for your account.")}</span>
        <MaterialUi.TextField value=`String(state.password)
          /* FIXME: when using wrapped TextField focus gets messed up, in the meantime use the wrapped styles here directly */
          className=([TextField.Styles.baseMargin, Styles.textField] >|< " ")
          _InputLabelProps=TextField.Styles.inputLabelProps
          _InputProps=TextField.Styles.inputProps

          name="password"
          type_="password"
          autoComplete="nope"
          label=ReasonReact.string("password")
          onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))
          />
        <MaterialUi.TextField value=`String(state.passwordConfirmation)
          /* FIXME: when using wrapped TextField focus gets messed up, in the meantime use the wrapped styles here directly */
          className=([TextField.Styles.baseMargin, Styles.textField] >|< " ")
          _InputLabelProps=TextField.Styles.inputLabelProps
          _InputProps=TextField.Styles.inputProps
          
          type_="password"
          autoComplete="nope"
          label=ReasonReact.string("confirm password")
          onChange=(event => dispatch(`PasswordConfirmationChanged(ReactEvent.Form.target(event)##value)))
          />
        <Button.Blended 
          className=Styles.button 
          onClick=(_event => dispatch(`CompleteNewPasswordRequest()))>
          "Confirm"
        </Button.Blended>
      </form>;

    let accountVerification = (~username, ~signInState: ReductiveCognito.signInState, ~state, ~retained, ~dispatch: action => unit) => {
      let verifying = switch(signInState){ | Verifying(_code, _username) => true | _ => false };
      let codeIncorrect = switch(signInState){ | AccountVerificationError(error, _code, _username) => error|.Amplify.Error.codeGet == "CodeMismatchException" | _ => false };
      let codeExpired = switch(signInState){ | AccountVerificationError(error, _, _) => error|.Amplify.Error.codeGet == "ExpiredCodeException" | _ => false };
      let resendingCode = switch(signInState){ | ResendingVerification(_) => true | _ => false };

      <form 
        className=Styles.form autoComplete="nope"
        ref=(element => {
          let _ = (element|.Js.Nullable.toOption)
          |. Belt.Option.map(element => {
            let focusIdx = String.length(state.verificationCode);
            let inputs = !!element
              |> ElementRe.querySelectorAll("input")
              |. NodeListRe.toArray;
            /* support return key to send */
            inputs |. Belt.Array.forEach(input => Rx.Observable.Operators.({
              let _ = Rx.Observable.fromEvent(input, "keydown")
                |> takeUntil(retained.willUnmount |. Rx.Subject.asObservable)
                |> filter((event: ReactEvent.Keyboard.t) => ReactEvent.Keyboard.keyCode(event) == 13)
                |> Rx.Observable.subscribe(~next=_event => {
                  dispatch(`SubmitVerificationCode(username))
                });
            }));
            /* 
              (for verification code reset) apply only when verification code is 1 digit
              FIXME: this is still hacky since user cannot errase by backspace completely
            */
            if(String.length(state.verificationCode) == 1 && focusIdx < Array.length(inputs)){
              Dom.HtmlElement.focus(!!(inputs[focusIdx]));
            }
          });
        })>
        <span className=Styles.welcomeTitle>{ReasonReact.string("Verify your account")}</span>
        <span className=Styles.accesoryLabel>{ReasonReact.string("We have send a verification code to your email address")}</span>
        <span className=merge([Styles.accesoryLabel, Styles.smallTopMargin])>{ReasonReact.string("please enter it here")}</span>
        <ReactCodeInput
          disabled=(verifying || codeExpired)
          value=state.verificationCode
          className=([Styles.codeInputBase, codeIncorrect || codeExpired ? Styles.errorCodeInput : Styles.normalCodeInput, verifying ? Styles.disabledCodeInput : ""] >|< " ")
          type_="number"
          fields=6
          onChange=(event => String.({
            /*
             * a bit hacky
             * since ReactCodeInput doesn't have a callback with each letter
             * on error we calculate a diff to find a changed letter and technically reset the input to it
             * but since FIXME: ReactCodeInput doesn't YET support two way binding (only initial value), we trigger the reductive action
             * which on signIn state change will result in full reinit of this component
             */
            if(codeIncorrect || codeExpired && state.verificationCode|.length == 6){
              let idxs = Array.init(min(event|.length, state.verificationCode|.length), x => x) |> Array.to_list;
              switch(
                idxs 
                |> List.find(idx => (event|.get(idx)) != (state.verificationCode|.get(idx)))){
              | exception Not_found => dispatch(`ForceVerificationRequired(make(1, event|.get(0)), username))
              | result => dispatch(`ForceVerificationRequired(make(1, event|.get(result)), username))
              }        
            } else {
              dispatch(`VerificationCodeChanged(event, username))
            }
          }))/>
        <span className=style([
          color(hsl(19, 100, 50)),
          fontFamily(Fonts.jost),
          height(px(24))])>
          {ReasonReact.string(
            codeExpired ? "code expired, please resend" :
            codeIncorrect ? "verification code incorrect" : "")
          }
        </span>

        {verifying || resendingCode
          ? <Button.Blended 
              disabled=true
              className=merge([Styles.button, style([opacity(0.7)])])
              onClick=(_event => ())>
              {resendingCode ? "resending..." : "verifying..."}
            </Button.Blended>
          : <Button.Blended 
              className=Styles.button
              onClick=(_event => dispatch(`ResendVerificationRequest(username)))>
              {"Resend verification code"}
            </Button.Blended>
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
      

  let make = (~state as signInState: ReductiveCognito.signInState, ~dispatch, ~mode, ~title, _children): ReasonReact.component(state, retained, action) => {
    ...ReasonReact.reducerComponentWithRetainedProps(__MODULE__),
    retainedProps: {
      formRef: None,
      emailRef: None,
      passwordRef: None,
      willUnmount: Rx.Subject.make()
    },

    initialState: () => {
      email: "",
      password: "",
      passwordConfirmation: "",
      staySignedIn: false,

      /**
       * FIXME: unfortunately each change to reductive state (signInState)
       * will result in full component recreation, thus to keep the verificationCode
       * during the state change to verifying we need to preserve it reductive and pass here
       */
      verificationCode: 
        switch(signInState){
        | AccountVerificationRequired(code, _)
        | Verifying(code, _)
        | AccountVerificationError(_, code, _) => code
        | _ => ""
        },

      showsAutofillInSignIn: false
    },

    didMount: self => Rx.Observable.Operators.({
      let _sub = didAutofillObservable(self.retainedProps)
      |> filter(value => value)
      |> takeUntil(self.retainedProps.willUnmount |. Rx.Subject.asObservable)
      |> Rx.Observable.subscribe(~next=_value => self.send(`ShowsAutofillInSignIn(true)));

      ReductiveDevTools.Connectors.register(
        ~connectionId=__MODULE__,
        ~component=self,
        ~options=ReductiveDevTools.Extension.enhancerOptions(()),
        ()
      )
    }),

    willUnmount: self => Rx.Subject.next(self.retainedProps.willUnmount, true),

    reducer: ReductiveDevTools.Connectors.componentReducerEnhancer(__MODULE__) @@ (action, state) =>
      switch (action) {
      | `EmailChanged(email) => ReasonReact.Update({ ...state, email, showsAutofillInSignIn: false })
      | `PasswordChanged(password) => ReasonReact.Update({ ...state, password, showsAutofillInSignIn: false })
      | `PasswordConfirmationChanged(passwordConfirmation) => ReasonReact.Update({ ...state, passwordConfirmation })
      | `DevToolStateUpdate(devToolsState) => ReasonReact.Update(devToolsState)
      | `StaySignedInChanged(staySignedIn) => ReasonReact.Update({ ...state, staySignedIn })
      | `SubmitVerificationCode(username) => 
        ReasonReact.SideEffects(_self => {
          String.length(state.verificationCode) == 6 
          ? dispatch(`ConfirmSignUpRequest(state.verificationCode, username))
          : ()
        })
      | `VerificationCodeChanged(verificationCode, username) => 
        String.length(verificationCode) == 6 
          ? ReasonReact.UpdateWithSideEffects(
            { ...state, verificationCode }, 
            _self => dispatch(`ConfirmSignUpRequest(verificationCode, username)))
          : ReasonReact.Update({ ...state, verificationCode })
      | `ShowsAutofillInSignIn(showsAutofillInSignIn) => ReasonReact.Update({ ...state, showsAutofillInSignIn })
      
      | `ForceVerificationRequired(code, username) => ReasonReact.SideEffects(_self => dispatch(`ForceVerificationRequired(code, username))) 
      | `RouterPushRoute(route) => ReasonReact.SideEffects(_self => dispatch(`RouterPushRoute(route)))
      | `CompleteNewPasswordRequest() => ReasonReact.SideEffects(_self => dispatch(`CompleteNewPasswordRequest(state.password)))
      | `ResendVerificationRequest(username) => ReasonReact.SideEffects(_self => dispatch(`ResendVerificationRequest(username)))
      | `SignOutRequest() => ReasonReact.SideEffects(_self => dispatch(`SignOutRequest(())))
      | `SignInRequest() => ReasonReact.SideEffects(_self => dispatch(`SignInRequest(state.email, state.password)))
      | `SignUpRequest() => ReasonReact.SideEffects(_self => 
          state.password == state.passwordConfirmation 
            ? dispatch(`SignUpRequest(state.email, state.password))
            : dispatch(`SignUpRequestRejected(Amplify.Error.t(~code="PasswordAndConfirmationDoNotMatchException", ~name="PasswordAndConfirmationDoNotMatchException", ~message="Password and confirmation do not match")))) 
      },

    render: ({ state, retainedProps, send }) => 
      <div className=Styles.root>
        <Logo.WithCaption 
          className=([Styles.logo, mode == SignUp ? Styles.hideLogoHackOnNarrowLayout : ""] >|< " ") 
          caption=title
          hideCaptionOnSmall=false/>
        
        <MaterialUi.Card className=Styles.card>
          {switch((mode, signInState)){
          | (_, SigningIn()) => <MaterialUi.CircularProgress size=`Int(128) className=Styles.progressSpinner/>
          | (SignIn, SignedIn(user)) when (user |. Amplify.Auth.CognitoUser.challengeNameGet) == Some("NEW_PASSWORD_REQUIRED") => 
            Forms.newPassword(state, send)
          | (VerifySignUp, AccountVerificationRequired(_, username))
          | (VerifySignUp, Verifying(_, username))
          | (VerifySignUp, ResendingVerification(username))
          | (VerifySignUp, AccountVerificationError(_, _, username)) => Forms.accountVerification(~username, ~signInState, ~state, ~retained=retainedProps, ~dispatch=send)
          | (VerifySignUp, _) => 
            <span className=Styles.welcomeTitle>{ReasonReact.string("Verification is not needed.")}</span>
          | (SignIn, SignedIn(_user)) => 
            <Fragment>
              <span className=Styles.welcomeTitle>{ReasonReact.string("You are signed in.")}</span>
              <Button.Blended 
                className=Styles.button
                onClick=(_event => send(`SignOutRequest(())))>
                {"Logout"}
              </Button.Blended>
            </Fragment>
          | (SignIn, _) => Forms.signIn(state, retainedProps, send)
          | (SignUp, _) => Forms.signUp(state, retainedProps, send)
          | (ForgotPassword, _) => Forms.forgotPassword(state, send)
          }}
        </MaterialUi.Card>
      </div>
  };
};

module UserProvider = {
  let make = Reductive.Lense.createMake(
    ~lense=(state: ReductiveCognito.withAuth(ReductiveRouter.withRouter(State.t))) => state.user, 
    Store.store);
};

let make = (~mode, ~title, _children) => {
  ...ReasonReact.statelessComponent(__MODULE__),
  render: _self => 
    <UserProvider component=Inner.make(~mode, ~title)/>
};