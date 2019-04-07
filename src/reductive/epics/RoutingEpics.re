open Operators;
open Rx.Observable.Operators;
open ReductiveObservable.Utils;
open Utils.Rx;

let goToDashboardAfterSuccessfulLogin = (reduxObservable: Rx.Observable.t(('action, 'state))) => Amplify.Auth.SignUpResult.(
  reduxObservable
  |> optMap(fun 
    | (`SignInCompleted(user), _) => Some(user) 
    | (`SignUpCompleted(signUpResult), _) when (signUpResult |. userConfirmedGet) => Some(signUpResult |. userGet)
    | _ => None)
  |> mergeMap((user: Amplify.Auth.CognitoUser.t) => {
    user |. Amplify.Auth.CognitoUser.challengeNameGet != Some("NEW_PASSWORD_REQUIRED") 
      ? Rx.Observable.of1(`RouterPushRoute(Routes.dashboard))
      : Rx.Observable.empty
  }));

let goToSignInAfterAccountConfirmation = (reduxObservable: Rx.Observable.t(('action, 'state))) =>
  reduxObservable
  |> optMap(fun | (`ConfirmSignUpCompleted(result), _) => Some(result) | _ => None)
  |> map(_result => `RouterPushRoute(Routes.signIn));

let goToSignInVerification = (reduxObservable: Rx.Observable.t(('action, 'state))) => Amplify.Auth.SignUpResult.(
  reduxObservable
  |> optMap(fun 
    | (`SignUpCompleted(signUpResult), _) when !(signUpResult |. userConfirmedGet) => Some(signUpResult |. userGet |. Amplify.Auth.CognitoUser.usernameGet)
    | (`SignInError(error, username), _) when (error |. Amplify.Error.codeGet == "UserNotConfirmedException") => Some(username)
    | _ => None)
  |> map(_username => `RouterPushRoute(Routes.verifySignUp))
);


let epic = reduxObservable => 
  Rx.Observable.merge([|
    reduxObservable |. goToDashboardAfterSuccessfulLogin,
    reduxObservable |. goToSignInAfterAccountConfirmation,
    reduxObservable |. goToSignInVerification
  |]);