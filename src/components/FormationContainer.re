open Operators;
open Css;
open Webapi;
open Rx.Observable.Operators;

let gridCellSize = 20;
let embedWidth = 520;
let embedHeight = 400;

let containedColor = raw("rgb(255, 255, 255)"|.AppTheme.rgbAddAlpha(0.25));

let formation: Formation.t = {
  name: "4-1-3-2",
  elements: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 4, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 9, y: 2, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 2, y: 3, xOffset: 0.0, yOffset: 0.0}) },
    { position: `CB, location: { x: 3, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 5, y: 4, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `CB, location: { x: 3, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 5, y: 6, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `RB, location: { x: 4, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 9, y: 8, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 2, y: 7, xOffset: 0.0, yOffset: 0.0}) },
    { position: `DM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 7, y: 5, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 3, y: 5, xOffset: 0.0, yOffset: 0.0}) },
    { position: `CM, location: { x: 7, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 8, y: 4, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 4, y: 3, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `CM, location: { x: 7, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 8, y: 6, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 4, y: 7, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `AM, location: { x: 9, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 6, y: 5, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `LW, location: { x: 11, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 6, y: 2, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `RW, location: { x: 11, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 6, y: 8, xOffset: 0.0, yOffset: 0.0 }) }
  |]
};

let defensiveFormation: Formation.t = {
  name: "4-1-3-2",
  elements: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 2, y: 3, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 2, y: 7, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 3, y: 5, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `AM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 6, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 6, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
  |]
};

let offensiveFormation: Formation.t = {
  name: "4-1-3-2",
  elements: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 9, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 5, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 5, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 9, y: 8, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 7, y: 5, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 8, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 8, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `AM, location: { x: 9, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 11, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 11, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
  |]
};

let squad: Team.squad = [|
  { name: "Allison", number: 13 },
  { name: "Robertson", number: 26 },
  { name: "Van Dijk", number: 4 },
  { name: "Matip", number: 32 },
  { name: "Gomes", number: 12 },
  { name: "Fabinho", number: 3 },
  { name: "Keita", number: 8 },
  { name: "Milner", number: 7 },
  { name: "Wijnaldum", number: 5 },
  { name: "Mane", number: 10 },
  { name: "Salah", number: 11 }
|];

module Styles {
  let root = style([
    height(px(embedHeight + 2)),
    width(px(embedWidth + 2)),
    border(px(1), `solid, rgba(255,255,255,0.2))
  ])
};

type retained = {
  mutable pixiContainerRef: option(Dom.HtmlElement.t)
};

type state = {
  scene: option(FormationRenderer.pitchScene)
}

type action = SetScene(option(FormationRenderer.pitchScene));

module BasePitchScene = FormationRenderer.BuildPitchScene(FormationSkin.BaseSkin);
module CompactPitchScene = FormationRenderer.BuildPitchScene(FormationSkin.CompactPresentationSkin);

let component = ReasonReact.reducerComponentWithRetainedProps(__MODULE__);
let make = (_children) => {
  ...component,
  retainedProps: {
    pixiContainerRef: None,
  },

  initialState: () => {
    scene: None
  },

  reducer: (action, state) => switch(action){
  | SetScene(scene) => ReasonReact.Update({ 
    ...state,
    scene
  })
  },

  didMount: self => {
    let pixiContainerRef = self.retainedProps.pixiContainerRef;
    Rx.Observable.merge([|
      FontFaceObserver.create(~fontFamily="Gobold", ()) |> FontFaceObserver.load |> Rx.Observable.fromPromise,
      FontFaceObserver.create(~fontFamily="Jost", ~options=FontFaceObserver.options(~weight=800, ()), ()) |> FontFaceObserver.load |> Rx.Observable.fromPromise
    |])
    |> Rx.Observable.Operators.bufferCount(2)
    |> Rx.Observable.Operators.take(1)
    |> Rx.Observable.subscribe(~next=_value =>
      Belt.Option.map(pixiContainerRef, containerRef => {
        let scene = BasePitchScene.create(~element=containerRef, ~width=embedWidth, ~height=embedHeight);
        self.send(SetScene(Some(scene)));
      }) |> ignore
    ) |> ignore
  },

  didUpdate: ({ oldSelf, newSelf }) => {
    newSelf.state.scene 
    |. Belt.Option.map(scene => { 
      let scene = scene |. BasePitchScene.loadFormation(~formation, ~team=Team.liverpool);
      
      /**
       * make sure all content is properly adjusted to a current zoom 
       * this is important during hot reload
       */
      BasePitchScene.handleZoom(scene.container);
      Rx.Observable.of1(scene)
      |> delay(5000)
      |> mergeMap(scene => 
        scene |> BasePitchScene.transitionToSkin(~skinBundle=FormationSkin.CompactPresentationSkin.bundle))
      |> mergeMap(scene =>
        scene |> BasePitchScene.transitionToFormation(~formation=offensiveFormation, ~labels=false))
      |> mergeMap(scene => 
        scene |> BasePitchScene.transitionToFormation(~formation=defensiveFormation, ~labels=false))
      |> mergeMap(scene => 
        scene |> BasePitchScene.transitionToFormation(~formation, ~labels=true))
      |> Rx.Observable.subscribe(~complete=() => ());
    })
    |> ignore;
  },

  render: ({ retainedProps }) => 
    <div className=Styles.root ref=(element => { retainedProps.pixiContainerRef = Js.Nullable.toOption(!!element) })>
    </div>
};