open Operators;
open Jest;
open JestDom;
open ReactTestingLibrary;
open TestUtils;
open Webapi.Dom;
open Reductive.Store;

describe("Auth SignIn", () => {
  open Expect;

  test("filling sign in form and submitting results in SignIn request", () => {
    let store = actionRecordingStore(redObs => redObs |> ReductiveObservable.Utils.empty);
    let auth = <Auth.Inner state=SignedOut(()) dispatch=dispatch(store) mode=Auth.SignIn title="Ballcast Tests"/>
    |> withIntl
    |> render;

    let emailInput = auth 
    |> getByClassName("test-sign-in-email-field")
    |> Element.querySelector("input") |> (?!!);

    let passwordInput = auth
    |> getByClassName("test-sign-in-password-field")
    |> Element.querySelector("input") |> (?!!);

    let submitButton = auth
    |> getByClassName("test-sign-in-button");

    /* Js.log(auth |> debug(~el=submitButton, ())); */
    emailInput |. FireEvent.change(!![%bs.obj { target: { value: "testbot" }}]);
    passwordInput |. FireEvent.change(!![%bs.obj { target: { value: "testtest" }}]);
    submitButton |. FireEvent.click;

    expect(store |. getState) |> toEqual([`SignInRequest("testbot", "testtest")])
  });

  test("clicking on sign in form sign up button triggers PushRoute action", () => {
    let store = actionRecordingStore(redObs => redObs |> ReductiveObservable.Utils.empty);
    <Auth.Inner state=SignedOut(()) dispatch=dispatch(store) mode=Auth.SignIn title="Ballcast Tests"/>
    |> withIntl
    |> render
    |> getByClassName("test-sign-in-alt-signup-button")
    |> FireEvent.click;
    expect(store |. getState) |> toEqual([`RouterPushRoute(Routes.signUp)])
  });
});

describe("Auth SignUp", () => {
  open Expect;

  test("filling sign in form and submitting results in SignIn request", () => {
    let store = actionRecordingStore(redObs => redObs |> ReductiveObservable.Utils.empty);
    let auth = <Auth.Inner state=SignedOut(()) dispatch=dispatch(store) mode=Auth.SignUp title="Ballcast Tests"/>
    |> withIntl
    |> render;

    let usernameInput = auth
    |> getByClassName("test-sign-up-username-field")
    |> Element.querySelector("input") |> (?!!);

    let passwordInput = auth
    |> getByClassName("test-sign-up-password-field")
    |> Element.querySelector("input") |> (?!!);

    let confirmPasswordInput = auth
    |> getByClassName("test-sign-up-confirmpassword-field")
    |> Element.querySelector("input") |> (?!!);

    let submitButton = auth
    |> getByClassName("test-sign-up-button");

    usernameInput |. FireEvent.change(!![%bs.obj { target: { value: "testbot" }}]);
    passwordInput |. FireEvent.change(!![%bs.obj { target: { value: "testtest" }}]);
    confirmPasswordInput |. FireEvent.change(!![%bs.obj { target: { value: "testtest" }}]);
    submitButton |. FireEvent.click;

    expect(store |. getState) |> toEqual([`SignUpRequest("testbot", "testtest")])
  });

  test("clicking on sign up form sign in button triggers PushRoute action", () => {
    let store = actionRecordingStore(redObs => redObs |> ReductiveObservable.Utils.empty);
    <Auth.Inner state=SignedOut(()) dispatch=dispatch(store) mode=Auth.SignUp title="Ballcast Tests"/>
    |> withIntl
    |> render
    |> getByClassName("test-sign-up-alt-signin-button")
    |> FireEvent.click;
    expect(store |. getState) |> toEqual([`RouterPushRoute(Routes.signIn)])
  });

});