open Rx.Operators;
open Utils.Rx;

// let goToDashboardAfterSuccessfulLogin = (reduxObservable: Rx.Observable.t(('action, 'state))) => Amplify.Auth.SignUpResult.(
//   reduxObservable
//   |> optMap(fun 
//     | (`SignInCompleted(user), _) => Some(user) 
//     | (`SignUpCompleted(signUpResult), _) when (signUpResult |. userConfirmedGet) => Some(signUpResult |. userGet)
//     | _ => None)
//   |> mergeMap(
//     `Observable((user: Amplify.Auth.CognitoUser.t, _idx) => 
//       user |. Amplify.Auth.CognitoUser.challengeNameGet != Some("NEW_PASSWORD_REQUIRED") 
//         ? Rx.of1(`RouterPushRoute(Routes.dashboard))
//         : Rx.empty
//     ),
//     ());

let goToSignInAfterAccountConfirmation = (reduxObservable: Rx.Observable.t(('action, 'state))) =>
  reduxObservable
  |> optMap(fun | (`ConfirmSignUpCompleted(result), _) => Some(result) | _ => None)
  |> map((_result, _idx) => `RouterPushRoute(Routes.signIn));

let goToSignInVerification = (reduxObservable: Rx.Observable.t(('action, 'state))) => AWSAmplify.Auth.SignUpResult.(
  reduxObservable
  |> optMap(fun 
    | (`SignUpCompleted(signUpResult), _) when !(signUpResult |. userConfirmedGet) => Some(signUpResult |. userGet |. AWSAmplify.Auth.CognitoUser.usernameGet)
    | (`SignInError(error, username), _) when (error |. AWSAmplify.Error.codeGet == "UserNotConfirmedException") => Some(username)
    | _ => None)
  |> map((_username, _idx) => `RouterPushRoute(Routes.verifySignUp))
);


let epic = reduxObservable => 
  Rx.merge([|
    // reduxObservable |. goToDashboardAfterSuccessfulLogin,
    reduxObservable |. goToSignInAfterAccountConfirmation,
    reduxObservable |. goToSignInVerification
  |]);