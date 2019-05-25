open Operators;
open Css;
open Webapi;

let gridCellSize = 20;
let embedWidth = 520;
let embedHeight = 400;

let containedColor = raw("rgb(255, 255, 255)"|.AppTheme.rgbAddAlpha(0.25));

[@bs.module] external pitchTexture: string = "assets/sprites/pitch_texture.png";
[@bs.module] external formationMarker: string = "assets/sprites/formation_marker_default.png";

let formation: Formation.t = {
  name: "4-1-3-2",
  elements: [|
    { position: Formation.GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.LB, location: { x: 3, y: 2, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.CB, location: { x: 3, y: 4, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.CB, location: { x: 3, y: 6, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.RB, location: { x: 3, y: 8, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.DM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.CM, location: { x: 7, y: 3, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.CM, location: { x: 7, y: 7, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.AM, location: { x: 9, y: 5, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.LW, location: { x: 11, y: 3, xOffset: 0.0, yOffset: 0.0 }},
    { position: Formation.RW, location: { x: 11, y: 7, xOffset: 0.0, yOffset: 0.0 }}
  |]
};

module Styles {
  let root = style([])
};

type retained = {
  mutable pixiContainerRef: option(Dom.HtmlElement.t)
};

let component = ReasonReact.statelessComponentWithRetainedProps(__MODULE__);
let make = (_children) => {
  ...component,
  retainedProps: {
    pixiContainerRef: None
  },

  didMount: self =>
    Belt.Option.map(self.retainedProps.pixiContainerRef, containerRef => {
      let formationRenderer = containerRef |. FormationRenderer.create(
        embedWidth, 
        embedHeight, 
        { pitchTexture, formationMarker });
      formationRenderer |. FormationRenderer.loadFormation(formation);
      formationRenderer
    })
    |> ignore,

  render: ({ retainedProps }) => 
    <div className=Styles.root ref=(element => { retainedProps.pixiContainerRef = Js.Nullable.toOption(!!element) })>
    </div>
};