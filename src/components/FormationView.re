open Operators;
open Css;
open Webapi;

let gridCellSize = 20;
let embedWidth = 520;
let embedHeight = 400;

let containedColor = raw("rgb(255, 255, 255)"|.AppTheme.rgbAddAlpha(0.25));

module PIXI {
  module Container { 
    type t;

    [@bs.module "pixi.js"][@bs.new]
    external create: unit => t = "Container";

    [@bs.send] external addChild: (t, t) => unit = "addChild";
  };

  module Application {
    module Options {
      [@bs.deriving abstract]
      type t = {
        [@bs.optional] sharedTicker: bool,
        [@bs.optional] sharedLoader: bool,
        [@bs.optional] autoStart: bool,
        [@bs.optional] width: int,
        [@bs.optional] height: int,
        [@bs.optional] view: Dom.HtmlElement.t,
        [@bs.optional] transparent: bool,
        [@bs.optional] antialias: bool,
        [@bs.optional] preserveDrawingBuffer: bool,
        [@bs.optional] resolution: float,
        [@bs.optional] forceCanvas: bool,
        [@bs.optional] backgroundColor: int,
        [@bs.optional] clearBeforeRender: bool,
        [@bs.optional] roundPixels: bool,
        [@bs.optional] forceFXAA: bool,
        [@bs.optional] legacy: bool
      }
    };

    type t;

    [@bs.get] external view: t => Dom.Element.t = "view";
    [@bs.get] external stage: t => Container.t = "stage";

    [@bs.module "pixi.js"][@bs.new]
    external create: (~options: Options.t=?, unit) => t = "Application";
  };
};

let initPixi = (element: Dom.HtmlElement.t) => PIXI.Application.({
  let application = PIXI.Application.create(~options=PIXI.Application.Options.t(
    ~transparent=true,
    ~width=embedWidth,
    ~height=embedHeight,
    ()), 
  ());

  element |> Dom.HtmlElement.appendChild(application|.view);
  let container = PIXI.Container.create();
  
  application |. stage |. PIXI.Container.addChild(container);
});

module Styles {
  let root = style([
    width(`px(embedWidth)),
    height(`px(embedHeight)),
    border(`px(1), `solid, containedColor),
    backgroundColor(containedColor)
  ])
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
    Belt.Option.map(self.retainedProps.pixiContainerRef, containerRef => containerRef |. initPixi) 
    |> ignore,

  render: ({ retainedProps }) => 
    <div className=Styles.root ref=(element => { retainedProps.pixiContainerRef = Js.Nullable.toOption(!!element) })></div>
};