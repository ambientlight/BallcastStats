open Jest;
open Operators;
open JestDom;
open ReactTestingLibrary;
open TestUtils;
open Webapi.Dom;

describe("NavHeader", () => {
  open Expect;
  test("Header contains title", () => 
    <NavHeader dispatch=((_action) => ()) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Str("Ballcast Tests")) |> (??)
    |> expect
    |> toBeInTheDocument
  );

  test("Header contains signUp button", () => 
    <NavHeader dispatch=((_action) => ()) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Func(( _text, node ) => 
      (node |> Element.tagName) === "BUTTON" && 
      (!!node |> Node.textContent |> String.trim) == "Sign Up")) 
    |> (??)
    |> expect
    |> toBeInTheDocument
  );

  test("Header contains signIn button", () => 
    <NavHeader dispatch=((_action) => ()) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Func(( _text, node ) => 
      (node |> Element.tagName) === "BUTTON" && 
      (!!node |> Node.textContent |> String.trim) == "Sign In")) 
    |> (??)
    |> expect
    |> toBeInTheDocument
  );

  ()
})