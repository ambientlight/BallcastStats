open Jest;
open Operators;
open JestDom;
open ReactTestingLibrary;
open TestUtils;
open Webapi.Dom;

describe("AppShell", () => {
  open Expect;
  
  test("contains cta button", () =>
    <Landing title="Ballcast Test" locale=Locale.En dispatch=(_action => ())/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Func(( _text, node ) => 
      (node |> Element.tagName) === "BUTTON" && 
      (!!node |> Node.textContent |> String.trim) == "Try now for free")) 
    |> expect
    |> toBeVisible
  );

  ()
});