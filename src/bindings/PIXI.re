open Webapi;

module Point {
  class type _t = [@bs] {
    [@bs.set] pub x: float;
    [@bs.set] pub y: float;
    pub set: (float, float) => unit;
    pub clone: unit => Js.t(_t);
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~x: float, ~y: float) => t = "Point";
};

module ObservablePoint {
  class type _t = [@bs] {
    [@bs.set] pub x: float;
    [@bs.set] pub y: float;
    pub set: (float, float) => unit;
    pub clone: (Js.t(_t) => unit, Js.t({..})) => unit;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~cb: t => unit, ~scope: Js.t({..}), ~x: option(float)=?, ~y: option(float)=?, unit) => t = "ObservablePoint";
};

module IPoint {
  type t = [
    | `Point(Point.t)
    | `ObservablePoint(ObservablePoint.t)
  ];

  [@bs.send]
  external copyPointFrom: (
    Point.t, 
    [@bs.unwrap] [`Point(Point.t) | `ObservablePoint(ObservablePoint.t)]
  ) => Point.t = "copyFrom";

  [@bs.send]
  external copyObservablePointFrom: (
    ObservablePoint.t, 
    [@bs.unwrap] [`Point(Point.t) | `ObservablePoint(ObservablePoint.t)]
  ) => ObservablePoint.t = "copyFrom";
  
  [@bs.send]
  external _observablePointEquals: (
    ObservablePoint.t,
    [@bs.unwrap] [`Point(Point.t) | `ObservablePoint(ObservablePoint.t)]
  ) => bool = "equals";

  [@bs.send]
  external _pointEquals: (
    Point.t,
    [@bs.unwrap] [`Point(Point.t) | `ObservablePoint(ObservablePoint.t)]
  ) => bool = "equals";

  [@bs.send]
  external _copyPointToPoint: (Point.t, Point.t) => Point.t = "copyTo";
  [@bs.send]
  external _copyPointToObservablePoint: (Point.t, ObservablePoint.t) => ObservablePoint.t = "copyTo";
  [@bs.send]
  external _copyObservablePointToPoint: (ObservablePoint.t, Point.t) => Point.t = "copyTo";
  [@bs.send]
  external _copyObservablePointToObservablePoint: (ObservablePoint.t, ObservablePoint.t) => ObservablePoint.t = "copyTo";


  let equal = (lhs: t, rhs: t) => 
    switch(lhs){
    | `ObservablePoint(point) => point |. _observablePointEquals(rhs)
    | `Point(point) => point |. _pointEquals(rhs)
    };

  let copyFrom = (lhs: t, rhs: t): t => 
    switch(lhs){
    | `ObservablePoint(point) =>  `ObservablePoint(point |. copyObservablePointFrom(rhs))
    | `Point(point) => `Point(point |. copyPointFrom(rhs))
    }; 
  
  let copyTo = (lhs: t, rhs: t): t => 
    switch(lhs, rhs){
      | (`ObservablePoint(lhsPoint), `ObservablePoint(rhsPoint)) => `ObservablePoint(lhsPoint |. _copyObservablePointToObservablePoint(rhsPoint))
      | (`ObservablePoint(lhsPoint), `Point(rhsPoint)) => `Point(lhsPoint |. _copyObservablePointToPoint(rhsPoint))
      | (`Point(lhsPoint), `ObservablePoint(rhsPoint)) => `ObservablePoint(lhsPoint |. _copyPointToObservablePoint(rhsPoint))
      | (`Point(lhsPoint), `Point(rhsPoint)) => `Point(lhsPoint |. _copyPointToPoint(rhsPoint))
      }; 
};

module DisplayObject {
  class type t = [@bs] {

    [@bs.set] pub accessible: bool;
    [@bs.set] pub accessibleHint: Js.Nullable.t(string);
    [@bs.set] pub accessibleTitle: Js.Nullable.t(string);
    [@bs.set] pub alpha: float;
    [@bs.set] pub angle: float;
    [@bs.set] pub buttonMode: bool;
    [@bs.set] pub cacheAsBitmap: bool;
    [@bs.set] pub cursor: string;
    /* [@bs.set] pub filterArea: Js.Nullable.t(Rectangle.t); */
    /* [@bs.set] pub filters: Js.Nullable.t(array(Filter.t)); */
    [@bs.set] pub hitArea: Js.t({..});
    pub interactive: bool;
    pub isSprite: bool;
    /* pub localTransform: Matrix.t; */
    [@bs.set] pub mask: Js.t({..})
    [@bs.set] pub name: string;
    pub parent: t;
    [@bs.set] pub pivot: Point.t;
    [@bs.set] pub position: Point.t;
    [@bs.set] pub renderable: bool;
    [@bs.set] pub rotation: float;
    [@bs.set] pub scale: Point.t;
    [@bs.set] pub skew: ObservablePoint.t;
    /* [@bs.set] pub transform: Transform.t; */
    [@bs.set] pub visible: bool;

    pub worldAlpha: float;
    /* pub worldTransform: Matrix.t; */
    
    [@bs.set] pub x: float;
    [@bs.set] pub y: float;
    [@bs.set] pub zIndex: int;
    
    pub destroy: unit => unit;
    /* pub getBounds: (bool, Rectangle.t) => Rectangle.t; */
  }
};

module ContainerC {
  class type t = {
    inherit DisplayObject.t;
  }
};

/* module ContainerC {
  [@bs]
  class virtual _t = {
    as _;
    inherit class DisplayObject.t as super;

    [@bs.meth] pub virtual doSomething: (float, float) => unit;
  }
  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: unit => t = "Container";
}; */

module Container { 
  type t;

  [@bs.get] external children: t => array(Js.Nullable.t(t)) = "children";
  [@bs.get] external name: t => Js.Nullable.t(string) = "name";
  [@bs.set] external setName: (t, string) => unit = "name";

  [@bs.module "pixi.js"][@bs.new]
  external create: unit => t = "Container";

  [@bs.send] external addChild: (t, t) => unit = "addChild";
  [@bs.send] external removeChild: (t, t) => unit = "removeChild";
};

module Texture {
  type t;
  
  [@bs.module "pixi.js"][@bs.scope "Texture"]
  external from: (~source: string) => t = "from";
}

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

  [@bs.get] external name: t => string = "name";
  [@bs.set] external setName: (t, string) => unit = "name";

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

  [@bs.get] external y: t => float = "y";
  [@bs.set] external setY: (t, float) => unit = "y";
  [@bs.get] external x: t => float = "x";
  [@bs.set] external setX: (t, float) => unit = "x";

  [@bs.get] external anchor: t => ObservablePoint.t = "anchor";
}