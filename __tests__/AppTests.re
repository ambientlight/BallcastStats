open Jest;
open Operators;
open JestDom;
open ReactTestingLibrary;
open TestUtils;
open Webapi.Dom;

describe("AppShell", () => {
  open Expect;
  
  test("shows signIn when ", () => {
    <App.Shell
      dispatch=((_action) => ())
      locale=Locale.En
      state=({ path:["sign-in"], hash:"", search:"" })
      title="Ballcast Tests"/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Str("Glad to see you back")) |> (??)
    |> expect
    |> toBeInTheDocument
  });

  ()
});