open Operators;
open Rx.Observable.Operators;
open ReductiveObservable.Utils;
open Utils.Rx;

let goToDashboardAfterSuccessfulLogin = (reduxObservable: Rx.Observable.t(('action, 'state))) =>
  reduxObservable
  |> optMap(fun | (`SignInCompleted(user), _) => Some(user) | _ => None)
  |> mergeMap((user: Amplify.Auth.CognitoUser.t) => {
    user |. Amplify.Auth.CognitoUser.challengeNameGet != Some("NEW_PASSWORD_REQUIRED") 
      ? Rx.Observable.of1(`RouterPushRoute(Routes.dashboard))
      : Rx.Observable.empty
  });

let goToSignInAfterAccountConfirmation = (reduxObservable: Rx.Observable.t(('action, 'state))) =>
  reduxObservable
  |> optMap(fun | (`ConfirmSignUpCompleted(user), _) => Some(user) | _ => None)
  |> map(_result => `RouterPushRoute(Routes.signIn));

let epic = reduxObservable => 
  Rx.Observable.merge([|
    reduxObservable |. goToDashboardAfterSuccessfulLogin,
    reduxObservable |. goToSignInAfterAccountConfirmation
  |]);