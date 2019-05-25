open Webapi;

module Container { 
  type t;

  [@bs.module "pixi.js"][@bs.new]
  external create: unit => t = "Container";

  [@bs.send] external addChild: (t, t) => unit = "addChild";
};

module Texture {
  type t;
  
  [@bs.module "pixi.js"][@bs.scope "Texture"]
  external from: (~source: string) => t = "from";
}

module ObservablePoint {
  type t;

  [@bs.get] external y: t => float = "y";
  [@bs.set] external setY: (t, float) => unit = "y";
  [@bs.get] external x: t => float = "x";
  [@bs.set] external setX: (t, float) => unit = "x";

  let setPosition = (t, ~x, ~y) => {
    t |. setX(x);
    t |. setY(y);
  };
};

module Sprite {
  type t;

  [@bs.module "pixi.js"][@bs.new]
  external create: Texture.t => t = "Sprite";  

  [@bs.send] external addChild: (t, t) => unit = "addChild";

  [@bs.get] external width: t => float = "width";
  [@bs.set] external setWidth: (t, float) => unit = "width";
  [@bs.get] external height: t => float = "height";
  [@bs.set] external setHeight: (t, float) => unit = "height";
  [@bs.get] external y: t => float = "y";
  [@bs.set] external setY: (t, float) => unit = "y";
  [@bs.get] external x: t => float = "x";
  [@bs.set] external setX: (t, float) => unit = "x";

  [@bs.get] external anchor: t => ObservablePoint.t = "anchor";

  let init = (~texture: Texture.t, 
      ~x: option(float)=?, ~y: option(float)=?, 
      ~width: option(float)=?, ~height: option(float)=?, ()) => {

    let sprite = create(texture);
    Belt.Option.map(x, x => sprite|.setX(x)) |> ignore;
    Belt.Option.map(y, y => sprite|.setY(y)) |> ignore;
    Belt.Option.map(width, width => sprite|.setWidth(width)) |> ignore;
    Belt.Option.map(height, height => sprite|.setHeight(height)) |> ignore;
    sprite
  }
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

  [@bs.get] external view: t => Dom.HtmlElement.t = "view";
  [@bs.get] external stage: t => Container.t = "stage";

  [@bs.module "pixi.js"][@bs.new]
  external create: (~options: Options.t=?, unit) => t = "Application";
};

module TextStyle {
  type t;

  [@bs.deriving abstract]
  type style = {
    [@bs.optional] fontFamily: array(string),
    [@bs.optional] fontSize: float,
    [@bs.optional] fill: int
  };

  [@bs.module "pixi.js"][@bs.new]
  external create: (~style: style) => t = "TextStyle";
};

module Text {
  type t;

  [@bs.module "pixi.js"][@bs.new]
  external create: (~text: string, ~style: TextStyle.t=?, ~canvas: HtmlElementRe.t=?, unit) => t = "Text";

  [@bs.get] external anchor: t => ObservablePoint.t = "anchor";
}