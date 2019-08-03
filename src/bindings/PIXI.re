open Webapi;

/**
 * PIXI reasonml bindings
 * 
 * following (experimental) guidelines were used:
 * 1. pixi's entities are modeled as class based Js.t for polymorphism with apis like .addChild() consuming subtypes seemlessly
 * 2. methods are both available as object methods as well as module-level functions. (object method bindings have few limitations: no support for overloaded method args and labeled args)
 * 3. union type fields like resizeTo: Window | HTMLElement are expressed as anonymous types and have corresponding module level getter/setter available that wraps the union type into polymorphic variant
 * 4. overloaded object method argument is expressed as anonymous type.
 * 5. object methods with arguments labeled as optionals and specifying default are not wrapped into Js.Undefined.t, otherwise option args are wrapped into Js.Undefined.t
 * 6. few helper modules are available corresponding to pixi types: PIXI.IPoint -> IPoint 
 */

let optMap = Belt.Option.map;

module TYPES {
  [@bs.deriving jsConverter]
  type t = 
    | [@bs.as 5121] UnsignedByte
    | [@bs.as 5123] UnsignedShort
    | [@bs.as 33635] UnsignedShort_5_6_5
    | [@bs.as 32819] UnsignedShort_4_4_4_4
    | [@bs.as 32820] UnsignedShort_5_5_5_1
    | [@bs.as 5126] Float
    | [@bs.as 36193] HalfFloat;

  let unsignedByte = 5121;
  let unsignedShort = 5123;
  let unsignedShort_5_6_5 = 33635;
  let unsignedShort_4_4_4_4 = 32819;
  let unsignedShort_5_5_5_1 = 32820;
  let float = 5126;
  let halfFloat = 36193;
};

module DRAW_MODES {
  [@bs.deriving jsConverter]
  type t = 
    | [@bs.as 0] Points
    | [@bs.as 1] Lines
    | [@bs.as 2] LineLoop
    | [@bs.as 3] LineStrip
    | [@bs.as 4] Triangles
    | [@bs.as 5] TriangleStrip
    | [@bs.as 6] TriangleFan;

  let points = 0;
  let lines = 1;
  let line_loop = 2;
  let line_strip = 3;
  let triangles = 4;
  let triangle_strip = 5;
  let triangle_fan = 6;
};

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
  type _use_IPoint_copyFrom;
  type _use_IPoint_copyTo;
  type _use_IPoint_equals;

  class type _t = [@bs] {
    [@bs.set] pub x: float;
    [@bs.set] pub y: float;
    pub set: (float, float) => unit;
    pub clone: unit => Js.t(_t);

    [@deprecated "Use IPoint.copyFrom"]
    pub copyFrom: _use_IPoint_copyFrom => Js.t(_t);
    [@deprecated "Use IPoint.copyTo"]
    pub copyTo: _use_IPoint_copyTo => _use_IPoint_copyTo;
    [@deprecated "Use IPoint.equals"]
    pub equals: _use_IPoint_equals => bool;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~x: float=?, ~y: float=?, unit) => t = "Point";

  [@bs.send]
  external set: (t, ~x: int=?, ~y: int=?, unit) => unit = "set";

  [@bs.send]
  external clone: t => t = "clone";
};

module ObservablePoint {
  type _use_IPoint_copyFrom;
  type _use_IPoint_copyTo;
  type _use_IPoint_equals;

  class type _t = [@bs] {
    [@bs.set] pub x: float;
    [@bs.set] pub y: float;
    pub set: (float, float) => unit;
    pub clone: (Js.t(_t) => unit, Js.t({..})) => unit;

    [@deprecated "Use IPoint.copyFrom"]
    pub copyFrom: _use_IPoint_copyFrom => Js.t(_t);
    [@deprecated "Use IPoint.copyTo"]
    pub copyTo: _use_IPoint_copyTo => _use_IPoint_copyTo;
    [@deprecated "Use IPoint.equals"]
    pub equals: _use_IPoint_equals => bool;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~cb: t => unit, ~scope: Js.t({..}), ~x: float=?, ~y: float=?, unit) => t = "ObservablePoint";

  [@bs.send]
  external set: (t, ~x: int=?, ~y: int=?, unit) => unit = "set";

  [@bs.send]
  external clone: t => t = "clone";
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

  [@bs.send]
  external ceil: (t, ~resolution:float=?, ~eps:float=?, unit) => unit = "ceil";

  [@bs.send]
  external clone: t => t = "unit";

  [@bs.send]
  external contains: (t, ~x: float, ~y: float) => bool = "contains";

  [@bs.send]
  external copyFrom: (t, ~rectangle: t) => t = "copyFrom";

  [@bs.send]
  external copyTo: (t, ~rectangle: t) => t = "copyTo";

  [@bs.send]
  external enlarge: (t, ~rectangle: t) => unit = "enlarge";

  [@bs.send]
  external fit: (t, ~rectangle: t) => unit = "fit";

  [@bs.send]
  external pad: (t, ~paddingX: float, ~paddingY: float) => unit = "pad";
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
  external create: (~program: Program.t=?, ~uniforms: Js.t({..})=?, unit) => Js.t(#_t) = "Shader";

  [@bs.module "pixi.js"][@bs.scope "Shader"]
  external from: (~vertexSrc: string=?, ~fragmentSrc: string=?, ~uniforms: Js.t({..})=?, unit) => Js.t(#_t) = "from";
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

module FilterSystem {
  type t;
};

module RenderTexture {
  type t;
}

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

    pub apply: (FilterSystem.t, RenderTexture.t, RenderTexture.t, bool, Js.Undefined.t(Js.t({..}))) => unit;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~vertexSrc: string=?, ~fragmentSrc: string=?, ~uniforms: Js.t({..})=?, unit) => Js.t(#_t) = "Filter";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external defaultFragmentSrc: string = "defaultFragmentSrc";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external defaultVertexSrc: string = "defaultVertexSrc";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external sourceKeyMap: Js.t({..}) = "SOURCE_KEY_MAP";

  [@bs.send]
  external apply: (Js.t(#_t), ~filterManager: FilterSystem.t, ~input: RenderTexture.t, ~output: RenderTexture.t, ~clear: bool, ~currentState: Js.t({..}) = ?, unit) => unit = "apply";
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
    pub apply: (Point.t, Js.Undefined.t(Point.t)) => Point.t;
    pub applyInverse: (Point.t, Js.Undefined.t(Point.t)) => Point.t;
    pub clone: unit => Js.t(_t);
    pub copyFrom: Js.t(_t) => Js.t(_t);
    pub copyTo: Js.t(_t) => Js.t(_t);
    pub decompose: Js.t({..}) => Js.t({..});
    pub fromArray: array(float) => unit;
    pub identity: unit => Js.t(_t);
    pub invert: unit => Js.t(_t);
    pub prepend: Js.t(_t) => Js.t(_t);
    pub rotate: float => Js.t(_t);
    pub scale: (float, float) => Js.t(_t);
    pub set: (float, float, float, float, float, float) => Js.t(_t);
    pub setTransform: (float, float, float, float, float, float, float, float, float) => Js.t(_t);
    pub toArray: (bool, Js.Undefined.t(Js.Typed_array.Float32Array.t)) => array(float);
    pub translate: (float, float) => Js.t(_t);
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~a:float=?, ~b:float=?, ~c:float=?, ~d:float=?, ~tx:float=?, ~ty:float=?, unit) => t = "Matrix";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Matrix"]
  external identity: t = "IDENTITY";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Matrix"]
  external tempMatrix: t = "TEMP_MATRIX";

  [@bs.send]
  external append: (t, ~matrix: t) => t = "append";

  [@bs.send]
  external apply: (t, ~pos: Point.t, ~newPos: Point.t=?, unit) => Point.t = "apply";

  [@bs.send]
  external applyInverse: (t, ~pos: Point.t, ~newPos: Point.t=?, unit) => Point.t = "applyInverse";

  [@bs.send]
  external clone: t => t = "clone";

  [@bs.send]
  external copyFrom: (t, ~matrix: t) => t = "copyFrom";

  [@bs.send]
  external copyTo: (t, ~matrix: t) => t = "copyTo";

  [@bs.send]
  external decompose: (t, ~transform: Js.t({..})) => Js.t({..}) = "decompose";

  [@bs.send]
  external fromArray: (t, ~array: array(float)) => unit = "fromArray";

  [@bs.send]
  external identity_: t => t = "identity";

  [@bs.send]
  external invert: t => t = "invert";

  [@bs.send]
  external prepend: (t, ~matrix: t) => t = "prepend";

  [@bs.send]
  external rotate: (t, ~angle: float) => t = "rotate";

  [@bs.send]
  external scale: (t, ~x: float, ~y: float) => t = "scale";

  [@bs.send]
  external set: (t, ~a: float, ~b: float, ~c: float, ~d: float, ~tx: float, ~ty: float) => t = "set";

  [@bs.send]
  external setTransform: (t, ~x: float, ~y: float, ~pivotX: float, ~pivotY: float, ~scaleX: float, ~scaleY: float, ~rotation: float, ~skewX: float, ~skewY: float) => t = "setTransform";

  [@bs.send]
  external toArray: (t, ~transpose: bool, ~out: Js.Typed_array.Float32Array.t=?, unit) => array(float) = "toArray";

  [@bs.send]
  external translate: (t, ~x: float, ~y: float) => t = "translate";
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

  [@bs.send]
  external setFromMatrix: (t, ~matrix: Matrix.t) => unit = "setFromMatrix";

  [@bs.send]
  external updateLocalTransform: t => unit = "updateLocalTransform";

  [@bs.send]
  external updateTransform: (t, ~parentTransform: t) => unit = "updateTransform";
};

module Renderer {
  type t;
}

module CanvasRenderer {
  type t;
}

module EventEmitter {
  class type _t = [@bs] {
    pub eventNames: array(string);
    pub listeners: array(Js.t({..}) => unit);

    pub listenerCount: array(string) => int;
    pub emit: string => bool;
    pub on: (string, Js.t({..}) => unit, Js.Undefined.t(Js.t({..}))) => Js.t(_t);
    pub addListener: (string, Js.t({..}) => unit, Js.Undefined.t(Js.t({..})) => unit, Js.Undefined.t(Js.t({..}))) => Js.t(_t);
    pub once: (string, Js.t({..}) => unit, Js.Undefined.t(Js.t({..}))) => Js.t(_t);
    pub removeListeners: (string, Js.Undefined.t(Js.t({..}) => unit), Js.Undefined.t(Js.t({..})), bool) => Js.t(_t);
    pub off: (string, Js.Undefined.t(Js.t({..}) => unit), Js.Undefined.t(Js.t({..})), bool) => Js.t(_t);
    pub removeAllListeners: Js.Undefined.t(string) => Js.t(_t);
  };

  type t = Js.t(_t);

  [@bs.send]
  external listenerCount: (Js.t(#_t), array(string)) => int = "listenerCount";

  [@bs.send]
  external emit: (Js.t(#_t), ~event: string) => bool = "emit";
  
  [@bs.send]
  external on: (Js.t(#_t as 'a), ~event: string, ~fn: Js.t({..}) => unit, ~context: Js.t({..})=?, unit) => Js.t('a) = "on";

  [@bs.send]
  external addListener: (Js.t(#_t as 'a), ~event: string, ~fn: Js.t({..}) => unit, ~context: Js.t({..})=?, unit) => Js.t('a) = "addListener";

  [@bs.send]
  external once: (Js.t(#_t as 'a), ~event: string, ~fn: Js.t({..}) => unit, ~context: Js.t({..})=?, unit) => Js.t('a) = "once";

  [@bs.send]
  external removeListeners: (Js.t(#_t as 'a), ~fn: (Js.t({..}) => unit)=?, ~context: Js.t({..})=?, ~once: bool=?, unit) => Js.t('a) = "removeListeners";

  [@bs.send]
  external off: (Js.t(#_t as 'a), ~fn: (Js.t({..}) => unit)=?, ~context: Js.t({..})=?, ~once: bool=?, unit) => Js.t('a) = "off";

  [@bs.send]
  external removeAllListeners: (Js.t(#_t as 'a), ~event: string=?, unit) => Js.t('a) = "removeAllListeners";
};

module DisplayObject {
  class type _t = [@bs] {
    inherit EventEmitter._t;

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
    [@bs.set] pub interactive: bool;
    pub isSprite: bool;
    pub localTransform: Matrix.t;

    /* TODO: PIXI.Graphics | PIXI.Sprite */
    [@bs.set] pub mask: Js.t({..});
    [@bs.set][@bs.get nullable] pub name: string;

    /* TODO: IPoint.t */
    [@bs.set] pub pivot: Point.t;
    [@bs.set] pub position: Point.t;

    [@bs.set] pub renderable: bool;
    [@bs.set] pub rotation: float;

    /* TODO: IPoint.t */
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

    /* TODO: IPoint.t */
    pub toGlobal: (Point.t, Js.Undefined.t(Point.t), bool) => Point.t;
    /* TODO: IPoint.t */
    pub toLocal: (Point.t, Js.Undefined.t(Js.t(_t)), Js.Undefined.t(Point.t), bool) => Point.t;
    pub updateTransform: unit => unit;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.scope "DisplayObject"]
  external mixin: (~source: Js.t({..})) => t = "mixin";

  [@bs.send]
  external _recursivePostUpdateTransform: t => unit = "_recursivePostUpdateTransform";

  [@bs.send]
  external destroy: t => unit = "destroy";

  [@bs.send]
  external getBounds: (t, ~skipUpdate: bool=?, ~rect: Rectangle.t=?, unit) => Rectangle.t = "getBounds";

  [@bs.send]
  external getGlobalPosition: (t, ~point: Point.t=?, ~skipUpdate: bool=?, unit) => Point.t = "getGlobalPosition";

  [@bs.send]
  external getLocalBounds: (t, ~rect: Rectangle.t=?, unit) => Rectangle.t = "getLocalBounds";

  [@bs.send]
  external render: (t, ~renderer: Renderer.t) => unit = "render";

  [@bs.send]
  external setTransform: (t, ~x: float=?, ~y: float=?, ~scaleX: float=?, ~scaleY: float=?, ~rotation: float=?, ~skewX: float=?, ~skewY: float=?, ~pivotX: float=?, ~pivotY: float=?, unit) => t = "_recursivePostUpdateTransform";

  /* TODO: returns IPoint.t */
  [@bs.send]
  external toGlobal: (t, ~position: [@bs.unwrap] [`Point(Point.t) | `ObservablePoint(ObservablePoint.t)], ~point: ([@bs.unwrap] [`Point(Point.t) | `ObservablePoint(ObservablePoint.t)])=?, ~skipUpdate: bool=?, unit) => Point.t = "toGlobal";

  /* TODO: returns IPoint.t */
  [@bs.send]
  external toLocal: (t, ~position: [@bs.unwrap] [`Point(Point.t) | `ObservablePoint(ObservablePoint.t)], ~from: t=?, ~point: ([@bs.unwrap] [`Point(Point.t) | `ObservablePoint(ObservablePoint.t)])=?, ~skipUpdate: bool=?, unit) => Point.t = "toLocal";

  [@bs.send]
  external updateTransform: t => unit = "updateTransform";
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
    pub removeChildren: (int, int) => array(DisplayObject.t);
    pub renderCanvas: CanvasRenderer.t => unit;
    pub setChildIndex: (Js.t(#DisplayObject._t), int) => unit;
    pub sortChildren: unit => unit;
    pub swapChildren: (Js.t(#DisplayObject._t), Js.t(#DisplayObject._t)) => unit;
  };

  [@bs.deriving abstract]
  type destroyOptions = {
    [@bs.optional] children: bool,
    [@bs.optional] texture: bool,
    [@bs.optional] baseTexture: bool
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: unit => t = "Container";

  [@bs.send]
  external addChild: (t, ~child: Js.t(#DisplayObject._t as 'a)) => Js.t('a) = "addChild";

  [@bs.send]
  external addChildAt: (t, ~child: Js.t(#DisplayObject._t as 'a), ~index: int) => Js.t('a) = "addChildAt";

  [@bs.send]
  external calculateBounds: t => unit = "calculateBounds";

  [@bs.send]
  external destroy: (t, ~option: ([@bs.unwrap] [`Object(destroyOptions) | `Bool(bool)])=?, unit) => unit = "destroy";

  [@bs.send]
  external getChildAt: (t, ~index: int) => Js.t(#DisplayObject._t) = "getChildAt";

  [@bs.send]
  external getChildByName: (t, ~name: string) => Js.Nullable.t(Js.t(#DisplayObject._t)) = "getChildByName";

  [@bs.send]
  external getChildIndex: (t, ~child: Js.t(#DisplayObject._t)) => int = "getChildIndex";

  [@bs.send]
  external removeChild: (t, ~child: Js.t(#DisplayObject._t as 'a)) => Js.t('a) = "removeChild";
  
  [@bs.send]
  external removeChildAt: (t, ~index: int) => DisplayObject.t = "removeChildAt";

  [@bs.send]
  external removeChildren: (t, ~beginIndex: int=?, ~endIndex: int=?, unit) => array(DisplayObject.t) = "removeChildren";

  [@bs.send]
  external renderCanvas: (t, ~renderer: CanvasRenderer.t) => unit = "renderCanvas";

  [@bs.send]
  external setChildIndex: (t, ~child: Js.t(#DisplayObject._t), ~index: int) => unit = "setChildIndex";

  [@bs.send]
  external sortChildren: t => unit = "sortChildren";

  [@bs.send]
  external swapChildren: (t, ~child: Js.t(#DisplayObject._t), ~child2: Js.t(#DisplayObject._t)) => unit = "swapChildren";

  [@bs.send]
  external getLocalBounds: (t, ~rect: Rectangle.t=?, unit) => Rectangle.t = "getLocalBounds";
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
    pub height: float;
    [@bs.set] pub noFrame: bool;
    [@bs.set] pub orig: Rectangle.t;
    [@bs.set] pub requiresUpdate: bool;
    [@bs.set] pub rotate: float;
    [@bs.set] pub textureCacheIds: array(string);
    [@bs.set] pub trim: Rectangle.t;
    [@bs.set] pub uvMatrix: TextureMatrix.t;
    [@bs.set] pub valid: bool;
    pub width: float;

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

  [@bs.send]
  external clone: t => unit = "clone";

  [@bs.send]
  external destroy: (t, ~destroyBase: bool=?, unit) => unit = "destroy";

  [@bs.send]
  external update: t => unit = "update";

  [@bs.send]
  external updateUvs: t => unit = "updateUvs";
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
    pub containsPoint: Point.t => bool;
  };
  type t = Js.t(_t);

  [@bs.deriving abstract]
  type destroyOptions = {
    [@bs.optional] children: bool,
    [@bs.optional] texture: bool,
    [@bs.optional] baseTexture: bool
  };

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
  };

  [@bs.send]
  external calculateTrimmedVertices: t => unit = "calculateTrimmedVertices";

  [@bs.send]
  external calculateVertices: t => unit = "calculateVertices";

  [@bs.send]
  external containsPoint: (t, ~point: Point.t) => bool = "containsPoint";

  [@bs.send]
  external destroy: (t, ~option: ([@bs.unwrap] [`Object(destroyOptions) | `Bool(bool)])=?, unit) => unit = "destroy";

  [@bs.send]
  external getLocalBounds: (t, ~rect: Rectangle.t=?, unit) => Rectangle.t = "getLocalBounds";
};

module TextStyle {
  type t;

  [@bs.deriving abstract]
  type style = {
    [@bs.optional] fontFamily: array(string),
    [@bs.optional] fontSize: float,
    [@bs.optional] fontWeight: string,
    [@bs.optional] fill: int,
    [@bs.optional] lineJoin: string,
    [@bs.optional] letterSpacing: float
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

  [@bs.deriving abstract]
  type destroyOptions = {
    [@bs.optional] children: bool,
    [@bs.optional] texture: bool,
    [@bs.optional] baseTexture: bool
  };

  [@bs.module "pixi.js"][@bs.new]
  external create: (~options: options=?, unit) => t = "Application";

  [@bs.module "pixi.js"][@bs.scope "Application"]
  external registerPlugin: (~plugin: plugin) => unit = "registerPlugin";

  [@bs.send]
  external destroy: (t, ~removeView: bool=?, ~stageOptions: ([@bs.unwrap] [`Object(destroyOptions) | `Bool(bool)])=?, unit) => unit = "destroy";

  [@bs.send]
  external render: t => unit = "render";

  [@bs.send]
  external resize: t => unit = "resize";

  [@bs.send]
  external start: t => unit = "start";

  [@bs.send]
  external stop: t => unit = "stop";
};

module InteractionManager {
  type t;
};

module BLEND_MODES {
  [@bs.deriving jsConverter]
  type t =
    | [@bs.as 0] Normal
    | [@bs.as 1] Add
    | [@bs.as 2] Multiply
    | [@bs.as 3] Screen
    | [@bs.as 4] Overlay
    | [@bs.as 5] Darken
    | [@bs.as 6] Lighten
    | [@bs.as 7] Color_dodge
    | [@bs.as 8] Color_burn
    | [@bs.as 9] Hard_light
    | [@bs.as 10] Soft_light
    | [@bs.as 11] Difference
    | [@bs.as 12] Exclusion
    | [@bs.as 13] Hue
    | [@bs.as 14] Saturation
    | [@bs.as 15] Color
    | [@bs.as 16] Luminosity
    | [@bs.as 17] Normal_npm
    | [@bs.as 18] Add_npm
    | [@bs.as 19] Screen_npm
    | [@bs.as 20] None
    /* TODO: [@bs.as 0] Src_over */
    | [@bs.as 21] Src_in
    | [@bs.as 22] Src_out
    | [@bs.as 23] Src_atop
    | [@bs.as 24] Dst_over
    | [@bs.as 25] Dst_in
    | [@bs.as 26] Dst_out
    | [@bs.as 27] Dst_atop
    /* TODO: [@bs.as 26] Erase */
    | [@bs.as 28] Subtract;


  let normal = 0;
  let add = 1;
  let multiply = 2;
  let screen = 3;
  let overlay = 4;
  let darken = 5;
  let lighten = 6;
  let color_dodge = 7;
  let color_burn = 8;
  let hard_light = 9;
  let soft_light = 10;
  let difference = 11;
  let exclusion = 12;
  let hue = 13;
  let saturation = 14;
  let color = 15;
  let luminosity = 16;
  let normal_npm = 17;
  let add_npm = 18;
  let screen_npm = 19;
  let none = 20;

  let src_over = 0;
  let src_in = 21;
  let src_out = 22;
  let src_atop = 23;
  let dst_over = 24;
  let dst_in = 25;
  let dst_out = 26;
  let dst_atop = 27;
  let erase = 26;
  let subtract = 28;
};

module Buffer {
  class type _t = {
    /* TODO: ArrayBuffer | SharedArrayBuffer | ArrayBufferView */
    pub data: Js.Typed_array.ArrayBuffer.t;
    pub destroy: unit => unit;
    pub dispose: unit => unit;
    pub update: Js.Typed_array.ArrayBuffer.t => unit;
  };

  type t = Js.t(_t);

  /* TODO: ArrayBuffer | SharedArrayBuffer | ArrayBufferView */
  [@bs.module "pixi.js"][@bs.new]
  external create: (~data: Js.Typed_array.ArrayBuffer.t, ~_static: bool=?, ~index: bool=?, unit) => t = "Buffer";

  [@bs.send]
  external destroy: t => unit = "destroy";

  [@bs.send]
  external dispose: t => unit = "dispose";

  /* TODO: ArrayBuffer | SharedArrayBuffer | ArrayBufferView */
  [@bs.send]
  external update: (t, Js.Typed_array.ArrayBuffer.t) => unit = "update";
};

module Attribute {
  class type _t = {
    pub from: (string, Js.Undefined.t(int), Js.Undefined.t(bool), Js.Undefined.t(int), Js.Undefined.t(int), Js.Undefined.t(int)) => Js.t(_t);
    pub destroy: unit => unit;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~buffer: string=?, ~size: int=?, ~normalized: bool=?, ~_type: int=?, ~stride: int=?, ~start: int=?, unit) => t = "Attribute";

  [@bs.send]
  external from: (t, ~buffer: string=?, ~size: int=?, ~stride: int=?, ~start: int=?, ~normalized: bool=?, unit) => t = "from";

  [@bs.send]
  external destroy: t => unit = "destroy";
};

module Geometry {
  class type _t = {
    pub refCount: int;

    pub addAttribute: (string, Js.Undefined.t(Buffer.t), Js.Undefined.t(int), Js.Undefined.t(bool), Js.Undefined.t(int), Js.Undefined.t(int), Js.Undefined.t(int)) => Js.t(_t);
    pub addIndex: Js.Undefined.t(Buffer.t) => Js.t(_t);
    pub clone: unit => Js.t(_t);
    pub destroy: unit => unit;
    pub dispose: unit => unit;
    pub getAttribute: string => Attribute.t;
    pub getBuffer: string => Buffer.t;
    pub getIndex: unit => Buffer.t;
    pub interleave: unit => Js.t(_t); 
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~buffers: array(Buffer.t)=?, ~attributes: Js.t({..})=?, unit) => t = "Geometry";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Geometry"]
  external merge: (~geometries: array(t)) => t = "merge";

  [@bs.send]
  external _addAttribute: (t, ~id: string, ~buffer: Buffer.t=?, ~size: int=?, ~normalized: bool=?, ~_type: int=?, ~stride: int=?, ~start: int=?, unit) => t = "addAttribute";

  let addAttribute = (t, ~id: string, ~buffer: option(Buffer.t)=?, ~size: option(TYPES.t)=?, ~normalized: option(bool)=?, ~_type: option(int)=?, ~stride: option(int)=?, ~start: option(int)=?, _t: unit) =>
    _addAttribute(t, ~id, ~buffer?, ~size=?switch(size){ | Some(size) => Some(TYPES.tToJs(size)) | _ => None }, ~normalized?, ~_type?, ~stride?, ~start?, ());

  [@bs.send]
  external addIndex: (t, ~buffer: Buffer.t=?, unit) => t = "addIndex";

  [@bs.send]
  external clone: t => t = "clone";

  [@bs.send]
  external destroy: t => unit = "destroy";

  [@bs.send]
  external dispose: t => unit = "dispose";

  [@bs.send]
  external getAttribute: (t, ~id: string) => Attribute.t = "getAttribute";

  [@bs.send]
  external getBuffer: (t, ~id: string) => Buffer.t = "getBuffer";

  [@bs.send]
  external getIndex: t => Buffer.t = "getIndex";

  [@bs.send]
  external interleave: t => t = "interleave";
};



module MeshMaterial {
  class type _t = {
    inherit Shader._t;

    [@bs.set] pub alpha: int;
    [@bs.set] pub batchable: bool;
    [@bs.set] pub pluginBatch: string;
    [@bs.set] pub texture: Texture.t;
    [@bs.set] pub tint: int;
    pub uvMatrix: TextureMatrix.t;

    pub update: unit => unit;
  };

  [@bs.deriving abstract]
  type meshMaterialOptions('a) = {
    [@bs.optional] alpha: int,
    [@bs.optional] tint: int,
    [@bs.optional] pluginName: string,
    [@bs.optional] program: Program.t,
    [@bs.optional] uniforms: Js.t({..} as 'a)
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~uSampler: Texture.t, ~options: meshMaterialOptions('a)=?, unit) => t = "MeshMaterial";

  [@bs.send]
  external update: t => unit = "update";
}

module Mesh {
  [@bs.deriving abstract]
  type destroyOptions = {
    [@bs.optional] children: bool
  };

  class type _t = {
    inherit Container._t;
    [@bs.set] pub blendMode: int;
    [@bs.set] pub drawMode: int;
    pub geometry: Geometry.t;
    [@bs.set] pub material: MeshMaterial.t;
    [@bs.set] pub roundPixels: bool;
    [@bs.set] pub shader: Shader.t;
    [@bs.set] pub size: float;
    [@bs.set] pub start: int;
    [@bs.set] pub state: State.t;
    [@bs.set] pub texture: Texture.t;
    [@bs.set] pub tint: int;
    pub uvBuffer: Buffer.t;
    pub verticesBuffer: Buffer.t;

    pub calculateUvs: unit => unit;
    pub calculateVertices: unit => unit;
    pub containsPoint: Point.t => bool;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external _create: (~geometry: Geometry.t, ~shader: [@bs.unwrap] [`Shader(Shader.t) | `MeshMaterial(MeshMaterial.t)], ~state: State.t=?, ~drawMode: int=?, unit) => Js.t(#_t) = "Mesh";

  let create = (~geometry: Geometry.t, ~shader: [@bs.unwrap] [`Shader(Shader.t) | `MeshMaterial(MeshMaterial.t)], ~state: option(State.t)=?, ~drawMode: option(DRAW_MODES.t)=?, unit) => 
    _create(~geometry, ~shader, ~state?, ~drawMode=?switch(drawMode){ | Some(mode) => Some(DRAW_MODES.tToJs(mode)) | _ => None }, ());

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Mesh"]
  external batchableSize: int = "BATCHABLE_SIZE";

  let getBlendMode: Js.t(#_t) => BLEND_MODES.t = entity => BLEND_MODES.tFromJs(entity##blendMode) |. Belt.Option.getExn;
  let getDrawMode: Js.t(#_t) => DRAW_MODES.t = entity => DRAW_MODES.tFromJs(entity##drawMode) |. Belt.Option.getExn;
  let setBlendMode = (entity: Js.t(#_t), blendMode: BLEND_MODES.t) => { entity##blendMode #= (blendMode |. BLEND_MODES.tToJs) };
  let setDrawMode = (entity: Js.t(#_t), drawMode: DRAW_MODES.t) => { entity##drawMode #= (drawMode |. DRAW_MODES.tToJs) };

  [@bs.send]
  external calculateUvs: Js.t(#_t) => unit = "calculateUvs";

  [@bs.send]
  external calculateVertices: Js.t(#_t) => unit = "calculateVertices";

  [@bs.send]
  external containsPoint: (Js.t(#_t), ~point: Point.t) => bool = "containsPoints";

  [@bs.send]
  external destroy: (Js.t(#_t), ~options: destroyOptions=?, unit) => unit = "destroy";
};

module SimpleRope {
  class type _t = {
    inherit Mesh._t;

    [@bs.set] pub autoUpdate: bool;
  };
  
  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~texture: Texture.t, ~points: array(Point.t)) => Js.t(#_t) = "SimpleRope";  
};

module GraphicsGeometry {
  type t;
};

module Graphics {
  class type _t = {
    inherit Container._t;

    [@bs.set] pub blendMode: int;
    [@bs.set] pub cacheAsBitmap: bool;
  };

  type t = Js.t(_t);

  [@bs.module "pixi.js"][@bs.new]
  external create: (~geometry: GraphicsGeometry.t=?, unit) => Js.t(#_t) = "Graphics";

  [@bs.send]
  external beginFill: (Js.t(#_t), ~color: int, ~alpha: float) => Js.t(#_t) = "beginFill";

  [@bs.send]
  external endFill: Js.t(#_t) => Js.t(#_t) = "endFill";

  [@bs.send]
  external drawCircle: (Js.t(#_t), ~x: float, ~y: float, ~radius: float) => Js.t(#_t) = "drawCircle";

  [@bs.send]
  external lineStyle: (Js.t(#_t), ~width: float=?, ~color: int=?, ~alpha: float=?, ~alignment: float=?, ~native: bool=?, unit) => Js.t(#_t) = "lineStyle";
};

module GraphicsCurves {

  [@bs.deriving abstract]
  type t = {
    mutable adaptive: bool,
    mutable maxLength: float,
    mutable minSegments: float,
    mutable maxSegments: float
  };
};

[@bs.module "pixi.js"]
external graphicsCurves: GraphicsCurves.t = "GRAPHICS_CURVES";
