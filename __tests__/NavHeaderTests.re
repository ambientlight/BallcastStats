open Jest;
open Operators;
open JestDom;
open ReactTestingLibrary;
open TestUtils;
open Webapi.Dom;
open Reductive.Store;

describe("NavHeader", () => {
  open Expect;
  test("Header contains title", () => 
    <NavHeader dispatch=((_action) => ()) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Str("Ballcast Tests")) |> (??)
    |> expect
    |> toBeVisible
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
    |> toBeVisible
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
    |> toBeVisible
  );

  test("clicking on signIn button triggers PushRoute action", () => {
    let store = actionRecordingStore(redObs => redObs |> ReductiveObservable.Utils.empty);
    <NavHeader dispatch=dispatch(store) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Func(( _text, node ) => 
      (node |> Element.tagName) === "BUTTON" && 
      (!!node |> Node.textContent |> String.trim) == "Sign In"))
    |> FireEvent.click;
    expect(store |. getState) |> toEqual([`RouterPushRoute(Routes.signIn)])
  });

  test("clicking on signUp button triggers PushRoute action", () => {
    let store = actionRecordingStore(redObs => redObs |> ReductiveObservable.Utils.empty);
    <NavHeader dispatch=dispatch(store) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByText(~matcher=`Func(( _text, node ) => 
      (node |> Element.tagName) === "BUTTON" && 
      (!!node |> Node.textContent |> String.trim) == "Sign Up"))
    |> FireEvent.click;
    expect(store |. getState) |> toEqual([`RouterPushRoute(Routes.signUp)])
  });

  ()
})