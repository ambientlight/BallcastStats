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
    |> getByClassName("test-navheader-signup-button") 
    |> (??)
    |> expect
    |> toBeVisible
  );

  test("Header contains signIn button", () => 
    <NavHeader dispatch=((_action) => ()) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByClassName("test-navheader-signin-button") 
    |> (??)
    |> expect
    |> toBeVisible
  );

  test("clicking on signIn button triggers PushRoute action", () => {
    let store = actionRecordingStore(redObs => redObs |> ReductiveObservable.Utils.empty);
    <NavHeader dispatch=dispatch(store) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByClassName("test-navheader-signin-button") 
    |> FireEvent.click;
    expect(store |. getState) |> toEqual([`RouterPushRoute(Routes.signIn)])
  });

  test("clicking on signUp button triggers PushRoute action", () => {
    let store = actionRecordingStore(redObs => redObs |> ReductiveObservable.Utils.empty);
    <NavHeader dispatch=dispatch(store) title="Ballcast Tests" locale=Locale.En/>
    |> withIntl
    |> render
    |> getByClassName("test-navheader-signup-button") 
    |> FireEvent.click;
    expect(store |. getState) |> toEqual([`RouterPushRoute(Routes.signUp)])
  });

  ()
})