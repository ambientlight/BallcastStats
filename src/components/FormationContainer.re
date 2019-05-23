open Operators;
open Css;
open Webapi;

let gridCellSize = 20;
let embedWidth = 520;
let embedHeight = 400;

let containedColor = raw("rgb(255, 255, 255)"|.AppTheme.rgbAddAlpha(0.25));

[@bs.module] external pitchTexture: string = "assets/sprites/pitch_texture.png";
[@bs.module] external formationMarker: string = "assets/sprites/formation_marker_default.png";

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
    Belt.Option.map(
      self.retainedProps.pixiContainerRef, 
      containerRef => containerRef |. FormationRenderer.create(
        embedWidth, 
        embedHeight, 
        { pitchTexture, formationMarker })
      ) 
    |> ignore,

  render: ({ retainedProps }) => 
    <div className=Styles.root ref=(element => { retainedProps.pixiContainerRef = Js.Nullable.toOption(!!element) })>
    </div>
};