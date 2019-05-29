open Webapi;

let optMap = Belt.Option.map;

module Shapes {
  [@bs.deriving jsConverter]
  type t =
    | [@bs.as 0] Poly
    | [@bs.as 1] Rect
    | [@bs.as 2] Circ
    | [@bs.as 3] Elip
    | [@bs.as 4] Rrec;
};

module Point {
  class type _t = [@bs] {
    [@bs.set] pub x: float;
    [@bs.set] pub y: float;
    pub set: (float, float) => unit;
    pub clone: unit => Js.t(_t);
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~x: float=?, ~y: float=?, unit) => t = "Point";
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
  external create: (~cb: t => unit, ~scope: Js.t({..}), ~x: float=?, ~y: float=?, unit) => t = "ObservablePoint";
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

module Rectangle {
  class type _t = [@bs] {
    [@bs.set] pub bottom: float;
    [@bs.set] pub height: float;
    [@bs.set] pub left: float;
    [@bs.set] pub right: float;
    [@bs.set] pub top: float;
    /* no [bs.as] support here for now, use module level accessor */
    /* [@bs.as "type"] pub type_: string; */
    [@bs.set] pub width: float;
    [@bs.set] pub x: float;
    [@bs.set] pub y: float;

    pub ceil: (float, float) => unit;
    pub clone: unit => Js.t(_t);
    pub contains: (float, float) => bool;
    pub copyFrom: Js.t(_t) => Js.t(_t);
    pub copyTo: Js.t(_t) => Js.t(_t);
    pub enlarge: Js.t(_t) => unit;
    pub fit: Js.t(_t) => unit;
    pub pad: (float, float) => unit;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (
    ~x: float=?, 
    ~y: float=?, 
    ~width: float=?, 
    ~height: float=?, 
    unit) => t = "Rectangle";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Rectangle"]
  external empty: t = "EMPTY";

  [@bs.get]
  external _getType: t => int = "type";
  let getType = rect => rect |. _getType |. Shapes.tFromJs;
};

module Program {
  class type _t = [@bs] {
    [@bs.set] pub fragmentSrc: string;
    [@bs.set] pub vertexSrc: string;
  };

  type t = Js.t(_t);
  
  [@bs.module "pixi.js"][@bs.new]
  external create: (~vertexSrc: string=?, ~fragmentSrc: string=?, ~name: string=?, unit) => t = "Program";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Program"]
  external defaultFragmentSrc: string = "defaultFragmentSrc";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Program"]
  external defaultVertexSrc: string = "defaultVertexSrc";

  [@bs.module "pixi.js"][@bs.scope "Program"]
  external from: (~vertexSrc: string=?, ~fragmentSrc: string=?, ~uniforms: Js.t({..})=?, unit) => t = "from";
};

module Shader {
  class type _t = [@bs] {
    pub uniforms: Js.t({..});
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~program: Program.t=?, ~uniforms: Js.t({..})=?, unit) => t = "Shader";

  [@bs.module "pixi.js"][@bs.scope "Shader"]
  external from: (~vertexSrc: string=?, ~fragmentSrc: string=?, ~uniforms: Js.t({..})=?, unit) => t = "from";
};

module State {
  class type _t = [@bs] {
    [@bs.set] pub blend: bool;
    [@bs.set] pub blendMode: bool;
    [@bs.set] pub clockwiseFrontFace: bool;
    [@bs.set] pub culling: bool;
    [@bs.set] pub depthTest: bool;
    [@bs.set] pub offsets: bool;
    [@bs.set] pub polygonOffset: float;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: unit => t = "State";
};

module Filter {
  class type _t = [@bs] {
    inherit Shader._t;

    [@bs.set] pub autoFit: bool;
    [@bs.set] pub blendMode: int;
    [@bs.set] pub enabled: bool;
    pub legacy: bool;
    [@bs.set] pub padding: float;
    [@bs.set] pub resolution: float;
    [@bs.set] pub state: State.t;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~vertexSrc: string=?, ~fragmentSrc: string=?, ~uniforms: Js.t({..})=?, unit) => t = "Filter";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external defaultFragmentSrc: string = "defaultFragmentSrc";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external defaultVertexSrc: string = "defaultVertexSrc";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external sourceKeyMap: Js.t({..}) = "SOURCE_KEY_MAP";
};

module Matrix {
  class type _t = [@bs] {
    [@bs.set] pub a: float;
    [@bs.set] pub b: float;
    [@bs.set] pub c: float;
    [@bs.set] pub d: float;
    [@bs.set] pub tx: float;
    [@bs.set] pub ty: float;

    pub append: Js.t(_t) => Js.t(_t);
    pub apply: (Js.t(_t), Js.Undefined.t(Js.t(_t))) => Js.t(_t);
    pub applyInverse: (Js.t(_t), Js.Undefined.t(Js.t(_t))) => Js.t(_t);
    pub clone: unit => Js.t(_t);
    pub copyFrom: Js.t(_t) => Js.t(_t);
    pub copyTo: Js.t(_t) => Js.t(_t);
    /* pub decompose: Transform.t => Transform.t; */
    pub fromArray: array(float) => unit;
    pub identity: unit => Js.t(_t);
    pub invert: unit => Js.t(_t);
    pub prepend: Js.t(_t) => Js.t(_t);
    pub rotate: float => Js.t(_t);
    pub scale: (float, float) => Js.t(_t);
    pub set: (float, float, float, float, float, float) => Js.t(_t);
    pub setTransform: (float, float, float, float, float, float, float, float, float) => Js.t(_t);
    pub toArray: (bool, Js.Undefined.t(array(float))) => array(float);
    pub translate: (float, float) => Js.t(_t);
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~a:float=?, ~b:float=?, ~c:float=?, ~d:float=?, ~tx:float=?, ~ty:float=?, unit) => t = "Matrix";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Matrix"]
  external identity: t = "IDENTITY";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Matrix"]
  external tempMatrix: t = "TEMP_MATRIX";
};

module Transform {
  class type _t = [@bs] {
    [@bs.set] pub localTranform: Matrix.t;
    [@bs.set] pub pivot: ObservablePoint.t;
    [@bs.set] pub position: ObservablePoint.t;
    [@bs.set] pub rotation: float;
    [@bs.set] pub scale: ObservablePoint.t;
    [@bs.set] pub skew: ObservablePoint.t;
    [@bs.set] pub worldTransform: Matrix.t;

    pub setFromMatrix: Matrix.t => unit;
    pub updateLocalTransform: unit => unit;
    pub updateTransform: Js.t(_t) => unit;
  };

  type t = Js.t(_t);

  [@bs.module][@bs.new]
  external create: unit => t = "Transform";
};

class type _displayObject = [@bs] {
  /* ... */
  [@bs.set] pub parent: Js.t(_container);
} and _container {
  inherit _displayObject;
  [@bs.set] pub something: int;
}

module Renderer {
  type t;
}

module CanvasRenderer {
  type t;
}

module DisplayObject {
  class type _t = [@bs] {
    [@bs.set] pub accessible: bool;
    [@bs.set] pub accessibleHint: Js.Nullable.t(string);
    [@bs.set] pub accessibleTitle: Js.Nullable.t(string);
    [@bs.set] pub alpha: float;
    [@bs.set] pub angle: float;
    [@bs.set] pub buttonMode: bool;
    [@bs.set] pub cacheAsBitmap: bool;
    [@bs.set] pub cursor: string;
    [@bs.set] pub filterArea: Js.Nullable.t(Rectangle.t);
    [@bs.set] pub filters: Js.Nullable.t(array(Filter.t));
    [@bs.set] pub hitArea: Js.t({..});
    pub interactive: bool;
    pub isSprite: bool;
    pub localTransform: Matrix.t;
    [@bs.set] pub mask: Js.t({..});
    [@bs.set][@bs.get nullable] pub name: string;

    [@bs.set] pub pivot: Point.t;
    [@bs.set] pub position: Point.t;
    [@bs.set] pub renderable: bool;
    [@bs.set] pub rotation: float;
    [@bs.set] pub scale: Point.t;
    [@bs.set] pub skew: ObservablePoint.t;
    [@bs.set] pub transform: Transform.t;
    [@bs.set] pub visible: bool;
  
    pub worldAlpha: float;
    pub worldTransform: Matrix.t;
    
    [@bs.set] pub x: float;
    [@bs.set] pub y: float;
    [@bs.set] pub zIndex: int;
    
    pub _recursivePostUpdateTransform: unit => unit;
    pub destroy: unit => unit;
    pub getBounds: (bool, Rectangle.t) => Rectangle.t;
    pub getGlobalPosition: (Point.t, bool) => Point.t;
    pub getLocalBounds: Js.Undefined.t(Rectangle.t) => Rectangle.t;
    pub render: Renderer.t => unit;
    pub setTransform: (float, float, float, float, float, float, float, float, float) => Js.t(_t);
    pub toGlobal: (Point.t, Js.Undefined.t(Point.t), bool) => Point.t;
    pub toLocal: (Point.t, Js.Undefined.t(Js.t(_t)), Js.Undefined.t(Point.t), bool) => Point.t;
    pub updateTransform: unit => unit;
  }

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.scope "DisplayObject"]
  external mixin: (~source: Js.t({..})) => t = "mixin";
};

module Container {
  class type _t = [@bs] {
    inherit DisplayObject._t;

    /* cannot be defined on DisplayObject for now since it seems we are not able to use [@bs] with s in class type p = {} and s {}*/
    pub parent: Js.t(_t);
    pub setParent: Js.t(_t) => Js.t(_t);

    [@bs.set] pub children: array(DisplayObject.t);
    [@bs.set] pub height: float;
    [@bs.set] pub interactiveChildren: bool;
    [@bs.set] pub sortableChildren: bool;
    [@bs.set] pub sortDirty: bool;
    [@bs.set] pub width: float;

    pub addChild: Js.t(#DisplayObject._t as 'a) => Js.t('a);
    pub addChildAt: (Js.t(#DisplayObject._t as 'a), int) => Js.t('a);
    pub calculateBounds: unit => unit;
    pub getChildAt: int => Js.t(#DisplayObject._t);
    pub getChildByName: string => Js.Nullable.t(Js.t(#DisplayObject._t));
    pub getChildIndex: Js.t(#DisplayObject._t) => int;
    pub removeChild: Js.t(#DisplayObject._t as 'a) => Js.t('a);
    pub removeChildAt: int => DisplayObject.t;
    pub removeChildren: (int, int) => array(Js.t(#DisplayObject._t));
    pub renderCanvas: CanvasRenderer.t => unit;
    pub setChildIndex: (Js.t(#DisplayObject._t), int) => unit;
    pub sortChildren: unit => unit;
    pub swapChildren: (Js.t(#DisplayObject._t), Js.t(#DisplayObject._t)) => unit;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: unit => t = "Container";
};

module BaseTexture {
  type t;
};

module TextureMatrix {
  type t;
};

module Texture {
  class type _t = [@bs] {
    [@bs.set] pub _frame: Rectangle.t;
    [@bs.set] pub baseTexture: BaseTexture.t;
    [@bs.set] pub defaultAnchor: Point.t;
    [@bs.set] pub frame: Rectangle.t;
    [@bs.set] pub height: float;
    [@bs.set] pub noFrame: bool;
    [@bs.set] pub orig: Rectangle.t;
    [@bs.set] pub requiresUpdate: bool;
    [@bs.set] pub rotate: float;
    [@bs.set] pub textureCacheIds: array(string);
    [@bs.set] pub trim: Rectangle.t;
    [@bs.set] pub uvMatrix: TextureMatrix.t;
    [@bs.set] pub valid: float;
    [@bs.set] pub width: float;

    pub clone: unit => unit;
    pub destroy: bool => unit;
    pub update: unit => unit;
    pub updateUvs: unit => unit;
  };
  type t = Js.t(_t);
  
  [@bs.val][@bs.module "pixi.js"][@bs.scope "Texture"]
  external empty: t = "EMPTY";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Texture"]
  external white: t = "WHITE";

  [@bs.module "pixi.js"][@bs.new]
  external create: (~baseTexture: BaseTexture.t, ~frame: Rectangle.t=?, ~orig: Rectangle.t=?, ~trim: Rectangle.t=?, ~rotate: float=?, ~anchor: Point.t=?, unit) => t = "Texture";

  [@bs.module "pixi.js"][@bs.scope "Texture"]
  external addToCache: (~texture: t, ~id: string) => unit = "addToCache";

  [@bs.module "pixi.js"][@bs.scope "Texture"]
  external from: (~source: string) => t = "from";

  [@bs.module "pixi.js"][@bs.scope "Texture"]
  external fromBuffer: (~buffer: [@bs.unwrap] [`Float32Array(Js.Typed_array.Float32Array.t) | `UInt8Array(Js.Typed_array.Uint8Array.t)], ~width: float, ~height: float, ~options: Js.t({..})=?, unit) => t = "fromBuffer";

  [@bs.module "pixi.js"][@bs.scope "Texture"]
  external fromLoader: (~source: Dom.HtmlImageElement.t, ~imageUrl: string, ~name: string=?, unit) => t = "fromLoader";

  [@bs.module "pixi.js"][@bs.scope "Texture"]
  external removeFromCache: (~texture: [@bs.unwrap] [`Texture(t) | `String(string)]) => Js.Nullable.t(t) = "removeFromCache";
}

module Sprite {
  class type _t = [@bs] {
    inherit Container._t;
    [@bs.set] pub anchor: ObservablePoint.t;
    [@bs.set] pub blendMode: int;
    [@bs.set] pub height: float;
    [@bs.set] pub pluginName: string;
    [@bs.set] pub roundPixels: bool;
    [@bs.set] pub shader: Js.t(#Shader._t);
    [@bs.set] pub texture: Texture.t;
    [@bs.set] pub tint: int;
    [@bs.set] pub width: float;

    pub calculateTrimmedVertices: unit => unit;
    pub calculateVertices: unit => unit;
    /* pub destroy: Js.t({..}) => unit; */
  };
  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: Texture.t => t = "Sprite";  

  let init = (~texture: Texture.t, 
      ~x: option(float)=?, ~y: option(float)=?, 
      ~width: option(float)=?, ~height: option(float)=?, ()) => {

    let sprite = create(texture);
    optMap(x, x => sprite##x #= x) |> ignore;
    optMap(y, y => sprite##y #= y) |> ignore;
    optMap(width, width => sprite##width #= width) |> ignore;
    optMap(height, height => sprite##height #= height) |> ignore;
    sprite
  }
};

module TextStyle {
  type t;

  [@bs.deriving abstract]
  type style = {
    [@bs.optional] fontFamily: array(string),
    [@bs.optional] fontSize: float,
    [@bs.optional] fill: int,
  };

  [@bs.module "pixi.js"][@bs.new]
  external create: (~style: style) => t = "TextStyle";
};

module Text {
  class type _t = [@bs] {
    inherit Sprite._t;

    [@bs.set] pub canvas: Dom.HtmlElement.t;
    [@bs.set] pub context: Webapi.Canvas.Canvas2d.t;
    [@bs.set] pub resolution: float;
    [@bs.set] pub style: TextStyle.t;
    [@bs.set] pub text: string;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~text: string, ~style: TextStyle.t=?, ~canvas: HtmlElementRe.t=?, unit) => t = "Text";
};

module Loader {
  type t;
}

module Ticker {
  type t;
}

module Application {
  [@bs.deriving abstract]
  type options = {
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
  };

  type plugin;

  class type _t = [@bs]{
    pub loader: Loader.t;
    [@bs.set] pub renderer: Renderer.t;
    [@bs.set] pub resizeTo: Dom.Window.t; /* or window */
    pub screen: Rectangle.t;
    [@bs.set] pub stage: Container.t;
    [@bs.set] pub ticker: Ticker.t;
    pub view: Dom.HtmlElement.t;

    pub destroy: (bool, Js.t({..})) => unit;
    pub render: unit => unit;
    pub resize: unit => unit;
    pub start: unit => unit;
    pub stop: unit => unit;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~options: options=?, unit) => t = "Application";

  [@bs.module "pixi.js"][@bs.scope "Application"]
  external registerPlugin: (~plugin: plugin) => unit = "registerPlugin";
};