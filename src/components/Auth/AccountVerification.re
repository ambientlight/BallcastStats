open Operators;
open Shortener;
open Css;
open Webapi;
open AuthLocale;
open AuthDefs;

module Styles = AuthStyles;

[@react.component]
let make = (~username, ~signInState: ReductiveCognito.signInState, ~state, ~retained: retained, ~dispatch: action => unit) => {
  let verifying = switch(signInState){ | Verifying(_code, _username) => true | _ => false };
  let codeIncorrect = switch(signInState){ 
    | AccountVerificationError(error, _code, _username) => error|.AWSAmplify.Error.codeGet == "CodeMismatchException" 
    | _ => false };
  let codeExpired = switch(signInState){ 
    | AccountVerificationError(error, _, _) => error|.AWSAmplify.Error.codeGet == "ExpiredCodeException" 
    | _ => false };
  let attemptLimitExceeded = switch(signInState){
    | AccountVerificationError(error, _, _) => error|.AWSAmplify.Error.codeGet == "LimitExceededException"
    | _ => false };
  
  let resendingCode = switch(signInState){ | ResendingVerification(_) => true | _ => false };
  let formRef: React.Ref.t(Js.Nullable.t(Dom.Element.t)) = React.useRef(Js.Nullable.null);
  let codeInputRef: React.Ref.t(Js.Nullable.t(Dom.Element.t)) = React.useRef(Js.Nullable.null);
  let shouldHighlightErrorCodeInput = (codeIncorrect || codeExpired || attemptLimitExceeded) && String.length(state.verificationCode) == 6;

  /**
    as soon as we filled in all 6 inputs in code input, reset and refocus on first 
   */
  React.useEffect0(() => {
    codeInputRef
    |> refc
    |. toopt
    |. optmap(element => {
      let resetAndFocus: Dom.Element.t => unit = [%bs.raw {|
        function (element) {
          if(element.state.value.length < 6){
            return
          }

          for(let i = 0; i < 6; i++){
            element.state.input[i] = "";  
          }
          
          element.textInput[0].focus();
        }
      |}];

      resetAndFocus(element);
    })
    |> ignore;

    None
  });

  <form 
    className=Styles.form autoComplete="nope"
    id="AuthVerifySignInForm"
    ref=ReactDOMRe.Ref.domRef(formRef)>

    <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
      <DefinedMessage message=strings##verifyYourAccount/>
    </MaterialUi.Typography>
    <MaterialUi.Typography variant=`Subtitle1>
      <DefinedMessage message=strings##verifyYourAccountDetail/>
    </MaterialUi.Typography>
    <MaterialUi.Typography variant=`Subtitle1 className=Styles.smallTopMargin>
      <DefinedMessage message=strings##pleaseEnterItHere/>
    </MaterialUi.Typography>
    <ReactCodeInput
      ref=ReactDOMRe.Ref.domRef(codeInputRef)
      disabled=(verifying || codeExpired)
      value=state.verificationCode
      className=([
        Styles.codeInputBase, 
        shouldHighlightErrorCodeInput ? Styles.errorCodeInput : Styles.normalCodeInput, 
        verifying ? Styles.disabledCodeInput : ""
        ] >|< " ")
      type_="number"
      fields=6
      onChange=(event => dispatch(`VerificationCodeChanged(event, username)))
      />
    <MaterialUi.Typography color=`Error variant=`Subtitle1 style=ReactDOMRe.Style.make(~height="24px", ())>
      {
        codeExpired && String.length(state.verificationCode) == 6 ? <DefinedMessage message=strings##codeExpiredPleaseResend/> :
        codeIncorrect && String.length(state.verificationCode) == 6 ? <DefinedMessage message=strings##verificationCodeIncorrect/> : 
        attemptLimitExceeded ? <DefinedMessage message=strings##attemptLimitExceeded/> : ReasonReact.string("")
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