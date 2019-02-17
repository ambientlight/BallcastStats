open Operators;
open Css;
open Webapi;

type mode = SignIn | SignUp | ForgotPassword;
module Styles = AuthStyles;

module Inner {
  /* storing element refs outside of component state since redux-dev-tools are having a bit hard time with them */
  type externalRefs = {
    mutable formRef: option(Dom.HtmlElement.t),
    mutable emailRef: option(Dom.HtmlElement.t),
    mutable passwordRef: option(Dom.HtmlElement.t)
  };

  let refs = {
    formRef: None,
    emailRef: None,
    passwordRef: None
  };

  type state = {
    email: string,
    password: string,
    passwordConfirmation: string,
    staySignedIn: bool,
    showsAutofillInSignIn: bool
  };

  type action = [ 
    | `EmailChanged(string)
    | `PasswordChanged(string)
    | `PasswordConfirmationChanged(string)
    | `StaySignedInChanged(bool)
    | `DevToolStateUpdate(state)
    | `SignInRequest(unit)
    | `RouterPushRoute(string)
    | `ShowsAutofillInSignIn(bool)
  ];
    
  let signInForm = (state, dispatch: action => unit) =>
    <form className=Styles.form ref=(element => { refs.formRef = Js.Nullable.toOption(!!element) })>
      <span className=Styles.welcomeTitle>{ReasonReact.string("Glad to see you back!")}</span>
      <MaterialUi.TextField value=`String(state.email)
        /* FIXME: when using wrapped TextField focus gets messed up, in the meantime use the wrapped styles here directly */
        className=([TextField.Styles.baseMargin, Styles.textField] >|< " ")
        _InputLabelProps=TextField.Styles.inputLabelProps
        _InputProps=TextField.Styles.inputProps

        inputRef=`Callback((element: Js.Nullable.t(Dom.HtmlElement.t)) => {
          refs.emailRef = Js.Nullable.toOption(element);
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
  
  let signUpForm = (state, dispatch: action => unit) =>
    <form className=Styles.form>
      <span className=Styles.welcomeTitle>{ReasonReact.string("Create your account")}</span>
      <TextField
        type_="email"
        label=ReasonReact.string("email")
        className=Styles.textField/>
      <TextField
        type_="password"
        label=ReasonReact.string("password")
        className=Styles.textField/>
      <TextField
        type_="password"
        label=ReasonReact.string("confirm password")
        className=Styles.textField/>
      <Button.Blended className=Styles.button>"Sign Up"</Button.Blended>
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

  let forgotPasswordForm = (state, dispatch: action => unit) =>
    <form className=Styles.form>
      <span className=Styles.welcomeTitle>{ReasonReact.string("Forgot your password?")}</span>
      <span className=merge([Styles.accesoryLabel, Styles.smallTopMargin])>{ReasonReact.string("Don't worry, please enter your email address, if there an account asssociated with it, we will send the reset password email to it shortly.")}</span>
      <TextField
        type_="email"
        label=ReasonReact.string("email")
        className=Styles.textField/>
      <Button.Blended className=Styles.button>"Send"</Button.Blended>
    </form>;

  let newPasswordForm = (state, dispatch: action => unit) =>
    <form className=Styles.form>
      <span className=Styles.welcomeTitle>{ReasonReact.string("New password required")}</span>
      <span className=merge([Styles.accesoryLabel, Styles.smallTopMargin])>{ReasonReact.string("You have logged in with a temporary password, you new to create a new password for your account.")}</span>
      <TextField
        type_="password"
        label=ReasonReact.string("password")
        className=Styles.textField/>
      <TextField
        type_="password"
        label=ReasonReact.string("confirm password")
        className=Styles.textField/>
      <Button.Blended 
        className=Styles.button 
        onClick=(_event => ())>
        "Confirm"
      </Button.Blended>
    </form>;

  let didAutofillObservable = Rx.Observable.Operators.(
    Rx.Observable.intervalFromScheduler(Rx.Scheduler.animationFrame)
    |> map(_value => 
      switch(refs.formRef){
      | Some(formRef) => 
        !!formRef
        |> ElementRe.querySelectorAll("input:-webkit-autofill")
        |. NodeListRe.toArray
        |> Array.fold_left((target, node) => {
          target
          || Belt.Option.eq(Some(node), refs.emailRef, (lhs, rhs) => !!lhs === rhs)
          || Belt.Option.eq(Some(node), refs.passwordRef, (lhs, rhs) => !!lhs === rhs)
        }, false)
      | None => false
      })
    |> filter(value => value)
    |> take(1));
      

  let make = (~state as signInState: ReductiveCognito.signInState, ~dispatch, ~mode, ~title, _children): ReasonReact.component(state, ReasonReact.noRetainedProps, action) => {
    ...ReasonReact.reducerComponent(__MODULE__),
    initialState: () => {
      email: "",
      password: "",
      passwordConfirmation: "",
      staySignedIn: false,
      showsAutofillInSignIn: false
    },

    didMount: self => Rx.Observable.Operators.({
      let _sub = didAutofillObservable
      |> filter(value => value)
      |> Rx.Observable.subscribe(~next=_value => self.send(`ShowsAutofillInSignIn(true)));

      ReductiveDevTools.Connectors.register(
        ~connectionId=__MODULE__,
        ~component=self,
        ~options=ReductiveDevTools.Extension.enhancerOptions(()),
        ()
      )
    }),

    reducer: ReductiveDevTools.Connectors.componentReducerEnhancer(__MODULE__) @@ (action, state) =>
      switch (action) {
      | `EmailChanged(email) => ReasonReact.Update({ ...state, email })
      | `PasswordChanged(password) => ReasonReact.Update({ ...state, password })
      | `PasswordConfirmationChanged(passwordConfirmation) => ReasonReact.Update({ ...state, passwordConfirmation })
      | `DevToolStateUpdate(devToolsState) => ReasonReact.Update(devToolsState)
      | `StaySignedInChanged(staySignedIn) => ReasonReact.Update({ ...state, staySignedIn })
      | `ShowsAutofillInSignIn(showsAutofillInSignIn) => ReasonReact.Update({ ...state, showsAutofillInSignIn })
      /* propagate actions to global store */
      | `RouterPushRoute(route) => ReasonReact.SideEffects(_self => dispatch(`RouterPushRoute(route)))
      | `SignInRequest() => ReasonReact.SideEffects(_self => dispatch(`SignInRequest(state.email, state.password))) 
      },

    render: ({ state, send }) => 
      <div className=Styles.root>
        <Logo.WithCaption 
          className=([Styles.logo, mode == SignUp ? Styles.hideLogoHackOnNarrowLayout : ""] >|< " ") 
          caption=title
          hideCaptionOnSmall=false/>
        
        <MaterialUi.Card className=Styles.card>
          {switch((mode, signInState)){
          | (_, SigningIn()) => <MaterialUi.CircularProgress size=`Int(128) className=Styles.progressSpinner/>
          | (_, SignedIn(user)) when (user |. Amplify.Auth.CognitoUser.challengeNameGet) == "NEW_PASSWORD_REQUIRED" => 
            newPasswordForm(state, send)
          | (_, SignedIn(_user)) => 
            <span className=Styles.welcomeTitle>{ReasonReact.string("You are signed in.")}</span>
          | (SignIn, _) => signInForm(state, send)
          | (SignUp, _) => signUpForm(state, send)
          | (ForgotPassword, _) => forgotPasswordForm(state, send)
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