open Jest;
open Operators;
open JestDom;
open ReactTestingLibrary;
open TestUtils;
open Webapi.Dom;

describe("AppShell", () => {
  open Expect;
  
  test("shows signIn in sign-in route", () =>
    <App.Shell
      dispatch=((_action) => ())
      locale=Locale.En
      state=({ path:["sign-in"], hash:"", search:"" })
      title="Ballcast Tests"/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Str("Glad to see you back")) |> (??)
    |> expect
    |> toBeVisible
  );

  test("shows signUp in sign-up route", () => 
    <App.Shell
      dispatch=((_action) => ())
      locale=Locale.En
      state=({ path:["sign-up"], hash:"", search:"" })
      title="Ballcast Tests"/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Str("Create your account")) |> (??)
    |> expect
    |> toBeVisible
  );

  test("shows forgotPassword in forgot route", () => 
    <App.Shell
      dispatch=((_action) => ())
      locale=Locale.En
      state=({ path:["forgot"], hash:"", search:"" })
      title="Ballcast Tests"/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Str("Forgot your password?")) |> (??)
    |> expect
    |> toBeVisible
  );

  test("shows verifySignIn in verify-sign-up route", () => 
    <App.Shell
      dispatch=((_action) => ())
      locale=Locale.En
      state=({ path:["verify-sign-up"], hash:"", search:"" })
      title="Ballcast Tests"/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Str("Verification is not needed.")) |> (??)
    |> expect
    |> toBeVisible
  );

  test("shows landing in root route", () => 
    <App.Shell
      dispatch=((_action) => ())
      locale=Locale.En
      state=({ path:[""], hash:"", search:"" })
      title="Ballcast Tests"/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Str("Embed it directly into your publication.")) |> (??)
    |> expect
    |> toBeVisible
  );

  ()
});