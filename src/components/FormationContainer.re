open Operators;
open Css;
open Webapi;

let gridCellSize = 20;
let embedWidth = 520;
let embedHeight = 400;

let containedColor = raw("rgb(255, 255, 255)"|.AppTheme.rgbAddAlpha(0.25));

[@bs.module] external pitchTexture: string = "assets/sprites/pitch_overscroll.png";
[@bs.module] external pitchTextureUniform: string = "assets/sprites/pitch_overscroll_uniform.png";
[@bs.module] external formationMarker: string = "assets/sprites/formation_marker_default.png";
[@bs.module] external formationMarkerThin: string = "assets/sprites/formation_marker_thin_corner_default.png";

let formation: Formation.t = {
  name: "4-1-3-2",
  elements: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 4, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 9, y: 2, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 2, y: 3, xOffset: 0.0, yOffset: 0.0}) },
    { position: `CB, location: { x: 3, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 5, y: 4, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `CB, location: { x: 3, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 5, y: 6, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `RB, location: { x: 4, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 9, y: 8, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 2, y: 7, xOffset: 0.0, yOffset: 0.0}) },
    { position: `DM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 7, y: 5, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 4, y: 5, xOffset: 0.0, yOffset: 0.0}) },
    { position: `CM, location: { x: 7, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 8, y: 4, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 6, y: 3, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `CM, location: { x: 7, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 8, y: 6, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 6, y: 7, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `AM, location: { x: 9, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 8, y: 5, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `LW, location: { x: 11, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 8, y: 2, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `RW, location: { x: 11, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 8, y: 8, xOffset: 0.0, yOffset: 0.0 }) }
  |]
};

let squad: Formation.squad = [|
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
|]

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
  renderer: option(FormationRenderer.t)
}

type action = SetFormationRenderer(option(FormationRenderer.t));

let component = ReasonReact.reducerComponentWithRetainedProps(__MODULE__);
let make = (_children) => {
  ...component,
  retainedProps: {
    pixiContainerRef: None,
  },

  initialState: () => {
    renderer: None
  },

  reducer: (action, state) => switch(action){
  | SetFormationRenderer(renderer) => ReasonReact.Update({ 
    ...state,
    renderer
  })
  },

  didMount: self => Rx.Observable.Operators.({
    let pixiContainerRef = self.retainedProps.pixiContainerRef;
    FontFaceObserver.create(~fontFamily="Gobold")
    |> FontFaceObserver.load
    |> Rx.Observable.fromPromise
    |> take(1)
    |> Rx.Observable.subscribe(~next=() =>
      Belt.Option.map(pixiContainerRef, containerRef => {
        let formationRenderer = containerRef |. FormationRenderer.create(
          embedWidth, 
          embedHeight, 
          { pitchTexture: pitchTexture, formationMarker });
        self.send(SetFormationRenderer(Some(formationRenderer)));
      }) |> ignore
    ) |> ignore
  }),

  didUpdate: ({ oldSelf, newSelf }) => {
    newSelf.state.renderer 
    |. Belt.Option.map(renderer => renderer |. FormationRenderer.loadFormation(formation, squad))
    |> ignore
  },

  render: ({ retainedProps }) => 
    <div className=Styles.root ref=(element => { retainedProps.pixiContainerRef = Js.Nullable.toOption(!!element) })>
    </div>
};