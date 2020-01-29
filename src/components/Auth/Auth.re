[%%debugger.chrome];

open Operators;
open Css;
open Webapi;
open AuthLocale;
open Utils.Tests;
open Shortener;

open AuthDefs;

module Styles = AuthStyles;

/** 
  NO LONGER USED (kept here in case we find this glitch somewhere else)
  used to circumvent a glitch where a form button is needed to be double-clicked
  when credentials auto-fill is applied (since the glitch was fixed)
  
  checks whether chrome autofill credentials was applied
  aggresively pins half-second to find :-webkit-autofill inputs
  in case these inputs match email/password input elements, emits true
*/
/*
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
*/

let authReducer = (state, action) =>
  switch (action) {
  | `EmailChanged(email) => ({ ...state, email, showsAutofillInSignIn: false })
  | `PasswordChanged(password) => ({ ...state, password, showsAutofillInSignIn: false })
  | `PasswordConfirmationChanged(passwordConfirmation) => ({ ...state, passwordConfirmation })
  | `DevToolStateUpdate(devToolsState) => { 
    devToolsState
  }
  | `StaySignedInChanged(staySignedIn) => ({ ...state, staySignedIn })
  | `VerificationCodeChanged(verificationCode, _username) => {
    { ...state, verificationCode }
  }
  | `ShowsAutofillInSignIn(showsAutofillInSignIn) => ({ ...state, showsAutofillInSignIn })
  | _ => state
};

/**
  wraps an auth reducer 
  with side-effect of propagating auth action to global reductive store
 */
let withPropagate = (dispatch, reducer) => (state, action) => {
  switch(action){
  | `SubmitVerificationCode(username) => {
    String.length(state.verificationCode) == 6 
      ? dispatch(`ConfirmSignUpRequest(state.verificationCode, username))
      : ();
    state 
  }
  | `VerificationCodeChanged(verificationCode, username) => {
    String.length(verificationCode) == 6
      ? dispatch(`ConfirmSignUpRequest(verificationCode, username))
      : ();
    // reducer also VerificationCodeChanged (not a pure side effect)
    reducer(state, action)
  }
  | `RouterPushRoute(route) => {
    dispatch(`RouterPushRoute(route));
    state
  }
  | `CompleteNewPasswordRequest() => {
    dispatch(`CompleteNewPasswordRequest(state.password));
    state
  }
  | `ResendVerificationRequest(username) => {
    dispatch(`ResendVerificationRequest(username));
    state
  }
  | `SignOutRequest() => {
    dispatch(`SignOutRequest(()));
    state
  }
  | `SignInRequest() => {
    dispatch(`SignInRequest(state.email, state.password));
    state
  }
  | `SignUpRequest() => {
    state.password == state.passwordConfirmation 
      ? dispatch(`SignUpRequest(state.email, state.password))
      : dispatch(`SignUpRequestRejected(Amplify.Error.t(
        ~code="PasswordAndConfirmationDoNotMatchException", 
        ~name="PasswordAndConfirmationDoNotMatchException", 
        ~message="Password and confirmation do not match")));
    state
  }
  | _ => reducer(state, action)
  };
};

module Inner {
  [@react.component]
  let make = (~state as signInState: ReductiveCognito.signInState, ~dispatch, ~mode: mode, ~title) => {
    let reducer = React.useMemo1(() => withPropagate(dispatch) @@ authReducer, [|dispatch|]);
    let (state, send) = ReductiveDevTools.Connectors.useReducer(
      ~options=ReductiveDevTools.Extension.enhancerOptions(
        ~name=__MODULE__,
        ~trace=true,
        ~actionCreators={
          "increment": () => `Increment(())
          |. ReductiveDevTools.Utilities.Serializer.serializeAction
        },
        ()),
      ~devToolsUpdateActionCreator=(state) => `DevToolStateUpdate(state),
      ~reducer,
      ~initial={
        accumulator: 0,
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
      }, ());

    let retained = React.useRef({
      mounted: false,
      formRef: None,
      emailRef: None,
      passwordRef: None,
      willUnmount: Rx.Subject.create()
    });

    /** 
      used to synchronize component lifecycle with retained.willUnmount
      that will be passed to inner components and used there to clean up their side-effects observables 
      (on this component unmount)
    */
    React.useEffect0(() => {
      if(!refc(retained).mounted){
        // used to circumvent a glitch where a form button is needed to be double-clicked
        // (kept here in case we find this glitch somewhere else)
        /*
        Rx.Observable.Operators.({
          didAutofillObservable(refc(retained))
          |> filter(value => value)
          |> takeUntil(refc(retained).willUnmount |. Rx.Subject.asObservable)
          |> Rx.Observable.subscribe(~next=_value => send(`ShowsAutofillInSignIn(true)))
          |> ignore
        });
        */

        refc(retained).mounted = true;
      };

      Some(() => {
        refc(retained).mounted = false;
        Rx.Subject.next(true, refc(retained).willUnmount);
      })
    });

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
          <NewPassword state dispatch=send/>

        | (VerifySignUp, AccountVerificationRequired(_, username))
        | (VerifySignUp, Verifying(_, username))
        | (VerifySignUp, ResendingVerification(username))
        | (VerifySignUp, AccountVerificationError(_, _, username)) => 
          <AccountVerification username retained=refc(retained) signInState state dispatch=send/>

        | (VerifySignUp, _) => 
          <div id="AuthVerifySignInForm">
            <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
              <DefinedMessage message=strings##verificationIsNotNeeded/>
            </MaterialUi.Typography>
          </div>

        | (SignIn, SignedIn(_user)) => 
          <>
            <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
              <DefinedMessage message=strings##youAreSignedIn/>
            </MaterialUi.Typography>
            <MaterialUi.Button variant=`Outlined 
              className=Styles.button
              onClick=(_event => send(`SignOutRequest(())))>
              <DefinedMessage message=CommonLocale.strings##logout/>
            </MaterialUi.Button>
          </>

        | (SignIn, _) => <SignIn state retained=refc(retained) dispatch=send/>
        | (SignUp, _) => <SignUp state retained=refc(retained) dispatch=send/>
        | (ForgotPassword, _) => <ForgotPassword state dispatch=send/>
        }}
      </MaterialUi.Card>
    </div>
  };
};

let authSelector = (state: Store.state) => state.state.user;

[@react.component]
let make = (~mode: mode, ~title) => {
  let user = AppStore.useSelector(authSelector);
  <Inner state=user dispatch=AppStore.useDispatch() mode title/>
};