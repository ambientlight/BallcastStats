open Webapi;

type mode = SignIn | SignUp | ForgotPassword | VerifySignUp;

type retained = {
  mutable mounted: bool,
  mutable formRef: option(Dom.Element.t),
  mutable emailRef: option(Dom.Element.t),
  mutable passwordRef: option(Dom.Element.t),
  willUnmount: Rx.Subject.t(bool)
};

type state = {
  accumulator: int,
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