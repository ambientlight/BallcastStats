open Operators;
open Jest;
open JestDom;
open ReactTestingLibrary;
open TestUtils;
open Webapi.Dom;
open Reductive.Store;

/* modules to be mocked here */
[@bs.module "aws-amplify"] external auth: Jestio.JestJs.moduleType = "Auth";

exception FailTest(string);

describe("Auth flow", () => {
  open Expect;

  testObservable("user can create an account, verify it and signIn", () => {
    auth |. Jestio.JestJs.assignMock("signUp", MockJs.fn(
      JestJs.fn((params: Amplify.Auth.SignUpParams.t) => {
        let user = Amplify.Auth.CognitoUser.t(~username=params |. Amplify.Auth.SignUpParams.usernameGet, ());
        let expectedSignUpResult = Amplify.Auth.SignUpResult.t(~user, ~userConfirmed=false);
        Js.Promise.resolve(expectedSignUpResult)
      })));

    auth |. Jestio.JestJs.assignMock("confirmSignUp", MockJs.fn(
      JestJs.fn((_username: string, _code: string) => Js.Promise.resolve(Js.Obj.empty()))
    ));

    auth |. Jestio.JestJs.assignMock("signIn", MockJs.fn(
      JestJs.fn((username: string, _password: string) => Js.Promise.resolve(
        Amplify.Auth.CognitoUser.t(~username, ~challengeName="", ())
      ))
    ));

    let app = <App.Root title="Ballcast Tests"/> |> render;
    let signUpButton = app |> getByClassName("test-navheader-signup-button");

    /* 1. navigate to sign up scene */
    signUpButton |. FireEvent.click;

    let usernameInput = app
    |> getByClassName("test-sign-up-username-field")
    |> Element.querySelector("input") |> (?!!);

    let passwordInput = app
    |> getByClassName("test-sign-up-password-field")
    |> Element.querySelector("input") |> (?!!);

    let confirmPasswordInput = app
    |> getByClassName("test-sign-up-confirmpassword-field")
    |> Element.querySelector("input") |> (?!!);

    let submitButton = app |> getByClassName("test-sign-up-button");

    /* 2. perform the sign up */
    usernameInput |. FireEvent.change(!![%bs.obj { target: { value: "testbot" }}]);
    passwordInput |. FireEvent.change(!![%bs.obj { target: { value: "testtest" }}]);
    confirmPasswordInput |. FireEvent.change(!![%bs.obj { target: { value: "testtest" }}]);
    submitButton |. FireEvent.click;

    DomTestingLibrary.waitForElement(~callback=() => app |> getById("AuthVerifySignInForm"), ())
    |> Rx.Observable.fromPromise
    |> Rx.Observable.Operators.mergeMap(signInForm => {
      /* 3. fill in verification code into codeInput (6 input elements) */
      signInForm
      |> Element.getElementsByTagName("input")
      |. HtmlCollection.toArray
      |. Belt.Array.forEach(input => input |. FireEvent.change(!![%bs.obj { target: { value: "0" }}]));
      DomTestingLibrary.waitForElement(~callback=() => app |> getByClassName("test-sign-in-email-field"), ()) |> Rx.Observable.fromPromise
    })
    |> Rx.Observable.Operators.mergeMap(emailField => {
      let emailInput = emailField
      |> Element.querySelector("input") |> (?!!);

      let passwordInput = app
      |> getByClassName("test-sign-in-password-field")
      |> Element.querySelector("input") |> (?!!);
      
      let submitButton = app
      |> getByClassName("test-sign-in-button");

      /* 4. perform sign in */
      emailInput |. FireEvent.change(!![%bs.obj { target: { value: "testbot" }}]);
      passwordInput |. FireEvent.change(!![%bs.obj { target: { value: "testtest" }}]);
      submitButton |. FireEvent.click;   
      DomTestingLibrary.waitForElement(~callback=() => app |> getById("Landing"), ()) |> Rx.Observable.fromPromise
    })
    |> Rx.Observable.Operators.map(_landing => {
      Jestio.Expect.expect(getState(Store.store).state.user) 
      |> Jestio.Expect.toEqual(ReductiveCognito.SignedIn(Amplify.Auth.CognitoUser.t(~username="testbot", ~challengeName="", ())))
    })
  });
})