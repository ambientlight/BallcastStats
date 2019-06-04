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
  external create: (~vertexSrc: string=?, ~fragmentSrc: string=?, ~uniforms: Js.t({..})=?, unit) => t = "Filter";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external defaultFragmentSrc: string = "defaultFragmentSrc";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external defaultVertexSrc: string = "defaultVertexSrc";

  [@bs.val][@bs.module "pixi.js"][@bs.scope "Filter"]
  external sourceKeyMap: Js.t({..}) = "SOURCE_KEY_MAP";

  [@bs.send]
  external apply: (t, ~filterManager: FilterSystem.t, ~input: RenderTexture.t, ~output: RenderTexture.t, ~clear: bool, ~currentState: Js.t({..}) = ?, unit) => unit = "apply";
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
  external on: (Js.t(#_t as 'a), ~event: string) => Js.t('a) = "on";

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
    pub interactive: bool;
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
  }

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
    [@bs.set] pub height: float;
    [@bs.set] pub noFrame: bool;
    [@bs.set] pub orig: Rectangle.t;
    [@bs.set] pub requiresUpdate: bool;
    [@bs.set] pub rotate: float;
    [@bs.set] pub textureCacheIds: array(string);
    [@bs.set] pub trim: Rectangle.t;
    [@bs.set] pub uvMatrix: TextureMatrix.t;
    [@bs.set] pub valid: bool;
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
}

module Viewport {
  module Plugin {
    type t;
  };

  [@bs.deriving abstract]
  type bounceOptionsEaseString = {
    [@bs.optional] sides: string,
    [@bs.optional] friction: float,
    [@bs.optional] time: int,
    [@bs.optional] ease: string,
    [@bs.optional] underflow: string
  };

  [@bs.deriving abstract]
  type bounceOptionsEaseFunction = {
    [@bs.optional] sides: string,
    [@bs.optional] friction: float,
    [@bs.optional] time: int,
    [@bs.optional] ease: (float, float, float, float) => float,
    [@bs.optional] underflow: string
  };

  [@bs.deriving abstract]
  type clampOptionsFloat = {
    [@bs.optional] left: float,
    [@bs.optional] right: float,
    [@bs.optional] top: float,
    [@bs.optional] bottom: float,
    [@bs.optional] direction: string,
    [@bs.optional] underflow: string
  };

  [@bs.deriving abstract]
  type clampOptionsBool = {
    [@bs.optional] left: bool,
    [@bs.optional] right: bool,
    [@bs.optional] top: bool,
    [@bs.optional] bottom: bool,
    [@bs.optional] direction: string,
    [@bs.optional] underflow: string
  };

  [@bs.deriving abstract]
  type clampZoomOptions = {
    [@bs.optional] minWidth: float,
    [@bs.optional] minHeight: float,
    [@bs.optional] maxWidth: float,
    [@bs.optional] maxHeight: float
  };

  [@bs.deriving abstract]
  type decelerateOptions = {
    [@bs.optional] friction: float,
    [@bs.optional] bounce: float,
    [@bs.optional] minSpeed: float
  };

  [@bs.deriving abstract]
  type dragOptionsClampWheelBool = {
    [@bs.optional] direction: string,
    [@bs.optional] wheel: bool,
    [@bs.optional] wheelScroll: int,
    [@bs.optional] reverse: bool,
    [@bs.optional] clampWheel: bool,
    [@bs.optional] underflow: string,
    [@bs.optional] factor: int,
    [@bs.optional] mouseButtons: string
  };

  [@bs.deriving abstract]
  type dragOptionsClampWheelString = {
    [@bs.optional] direction: string,
    [@bs.optional] wheel: bool,
    [@bs.optional] wheelScroll: int,
    [@bs.optional] reverse: bool,
    [@bs.optional] clampWheel: string,
    [@bs.optional] underflow: string,
    [@bs.optional] factor: int,
    [@bs.optional] mouseButtons: string
  };

  [@bs.deriving abstract]
  type followOptions = {
    [@bs.optional] speed: float,
    [@bs.optional] acceleration: float,
    [@bs.optional] radius: float
  };

  [@bs.deriving abstract]
  type bounds = {
    x: float,
    y: float,
    width: float,
    height: float
  };

  [@bs.deriving abstract]
  type mouseEdgesOptions = {
    [@bs.optional] radius: float,
    [@bs.optional] distance: float,
    [@bs.optional] top: float,
    [@bs.optional] bottom: float,
    [@bs.optional] left: float,
    [@bs.optional] right: float,
    [@bs.optional] speed: int,
    [@bs.optional] reverse: bool,
    [@bs.optional] noDecelerate: bool,
    [@bs.optional] linear: bool,
    [@bs.optional] allowButtons: bool
  };

  [@bs.deriving abstract]
  type pinchOptions = {
    [@bs.optional] percent: float,
    [@bs.optional] noDrag: bool,
    [@bs.optional] center: Point.t
  };

  [@bs.deriving abstract]
  type snapOptionsEaseString = {
    [@bs.optional] topLeft: bool,
    [@bs.optional] friction: float,
    [@bs.optional] time: int,
    [@bs.optional] ease: string,
    [@bs.optional] interrupt: bool,
    [@bs.optional] removeOnComplete: bool,
    [@bs.optional] removeOnInterrupt: bool,
    [@bs.optional] forceStart: bool
  };

  [@bs.deriving abstract]
  type snapOptionsEaseFunction = {
    [@bs.optional] topLeft: bool,
    [@bs.optional] friction: float,
    [@bs.optional] time: int,
    [@bs.optional] ease: (float, float, float, float) => float,
    [@bs.optional] interrupt: bool,
    [@bs.optional] removeOnComplete: bool,
    [@bs.optional] removeOnInterrupt: bool,
    [@bs.optional] forceStart: bool
  };

  [@bs.deriving abstract]
  type snapZoomOptionsEaseString = {
    [@bs.optional] width: float,
    [@bs.optional] height: float,
    [@bs.optional] time: int,
    [@bs.optional] ease: string,
    [@bs.optional] center: Point.t,
    [@bs.optional] interrupt: bool,
    [@bs.optional] removeOnComplete: bool,
    [@bs.optional] removeOnInterrupt: bool,
    [@bs.optional] forceStart: bool,
    [@bs.optional] noMove: bool
  };

  [@bs.deriving abstract]
  type snapZoomOptionsEaseFunction = {
    [@bs.optional] width: float,
    [@bs.optional] height: float,
    [@bs.optional] time: int,
    [@bs.optional] ease: (float, float, float, float) => float,
    [@bs.optional] center: Point.t,
    [@bs.optional] interrupt: bool,
    [@bs.optional] removeOnComplete: bool,
    [@bs.optional] removeOnInterrupt: bool,
    [@bs.optional] forceStart: bool,
    [@bs.optional] noMove: bool
  };

  [@bs.deriving abstract]
  type wheelOptions = {
    [@bs.optional] percent: float,
    [@bs.optional] reverse: bool,
    [@bs.optional] center: Point.t
  };

  [@bs.deriving abstract]
  type createOptions('a) = {
    [@bs.optional] screenWidth: float,
    [@bs.optional] screenHeight: float,
    [@bs.optional] worldWidth: float,
    [@bs.optional] worldHeight: float,
    [@bs.optional] threshold: int,
    [@bs.optional] passiveWheel: bool,
    [@bs.optional] stopPropagation: bool,
    /* TODO: PIXI.Rectangle | PIXI.Circle | PIXI.Ellipse | PIXI.Polygon | PIXI.RoundedRectangle */
    [@bs.optional] forceHitArea: 'a,
    [@bs.optional] noTicker: bool,
    [@bs.optional] ticker: Ticker.t,
    [@bs.optional] interaction: InteractionManager.t,
    [@bs.optional] divWheel: Dom.HtmlElement.t
  };

  class type _t = [@bs]{
    inherit Container._t;
    [@bs.set] pub bottom: float;
    [@bs.set] pub center: float;
    [@bs.set] pub corner: Point.t;
    [@bs.set] pub dirty: bool;
    /* TODO: PIXI.Rectangle | PIXI.Circle | PIXI.Ellipse | PIXI.Polygon | PIXI.RoundedRectangle */
    [@bs.set] pub forceHitArea: Js.t({..});
    [@bs.set] pub left: float;
    [@bs.set] pub pause: bool;
    [@bs.set] pub right: float;
    [@bs.set] pub screenWidth: float;
    pub screenWorldHeight: float;
    pub screenWorldWidth: float;
    [@bs.set] pub top: float;
    pub touches: array(float);
    [@bs.set] pub worldHeight: float;
    pub worldScreenHeight: float;
    pub worldScreenWidth: float;
    [@bs.set] pub worldWidth: float;

    pub bounce: bounceOptionsEaseString => Js.t(_t);
    pub clamp: clampOptionsBool => Js.t(_t);
    pub clampZoom: clampZoomOptions => Js.t(_t);
    pub decelerate: decelerateOptions => Js.t(_t);
    pub drag: dragOptionsClampWheelBool => Js.t(_t);
    pub ensureVisible: (float, float, float, float) => unit;
    pub fit: (bool, Js.Undefined.t(float), Js.Undefined.t(float)) => Js.t(_t);
    pub fitHeight: (Js.Undefined.t(float), bool, bool, bool) => Js.t(_t);
    pub fitWidth: (Js.Undefined.t(float), bool, bool, bool) => Js.t(_t);
    pub fitWorld: bool => Js.t(_t);
    pub follow: (Js.t(#DisplayObject._t), Js.Undefined.t(followOptions)) => Js.t(_t);
    pub getVisibleBounds: unit => bounds;
    pub mouseEdges: mouseEdgesOptions => Js.t(_t);
    pub moveCenter: (float, float) => Js.t(_t);
    pub pausePlugin: string => unit;
    pub pinch: pinchOptions => unit;
    pub removePlugin: string => unit;
    pub resize: (Js.Undefined.t(float), Js.Undefined.t(float), Js.Undefined.t(float), Js.Undefined.t(float)) => unit;
    pub resumePlugin: string => unit;
    pub snap: (float, float, snapOptionsEaseString) => Js.t(_t);
    pub snapZoom: snapZoomOptionsEaseString => Js.t(_t);
    pub toScreen: (float, float) => Point.t;
    pub toWorld: (float, float) => Point.t;
    pub update: unit => unit;
    pub userPlugin: (string, Plugin.t, int) => unit;
    pub wheel: wheelOptions => unit;
    pub zoom: (float, bool) => Js.t(_t);
    pub zoomPercent: (float, bool) => Js.t(_t);
  };

  type t = Js.t(_t);

  [@bs.module "pixi-viewport"][@bs.new]
  external create: (~options: createOptions('a)=?, unit) => t = "Viewport";
  
  [@bs.send]
  external bounce: (t, ~options: ([@bs.unwrap][`EaseString(bounceOptionsEaseString) | `EaseFunction(bounceOptionsEaseFunction)])=?, unit) => t = "bounce";

  [@bs.send]
  external clamp: (t, ~options: ([@bs.unwrap] [`FloatOptions(clampOptionsFloat) | `BoolOptions(clampOptionsBool)])=?, unit) => t = "clamp";

  [@bs.send]
  external decelerate: (t, ~options: decelerateOptions=?, unit) => t = "decelerate";

  [@bs.send]
  external drag: (t, ~options: ([@bs.unwrap] [`ClampWheelBool(dragOptionsClampWheelBool) | `ClampWheelString(dragOptionsClampWheelString)])=?, unit) => t = "drag";

  [@bs.send]
  external ensureVisible: (t, ~x: float, ~y: float, ~width: float, ~height: float) => unit = "ensureVisible";

  [@bs.send]
  external fit: (t, ~center: bool=?, ~width: float=?, ~height: float=?, unit) => t = "fit";

  [@bs.send]
  external fitHeight: (t, ~height: float=?, ~center: bool=?, ~scaleX: bool=?, ~noClamp: bool=?, unit) => t = "fitHeight";

  [@bs.send]
  external fitWidth: (t, ~width: float=?, ~center: bool=?, ~scaleX: bool=?, ~noClamp: bool=?, unit) => t = "fitHeight";

  [@bs.send]
  external fitWorld: (t, ~center: bool=?, unit) => t = "fitWorld";

  [@bs.send]
  external follow: (t, ~target: Js.t(#DisplayObject._t), ~options: followOptions=?, unit) => t = "follow";

  [@bs.send]
  external getVisibleBounds: t => bounds = "getVisibleBounds";

  [@bs.send]
  external mouseEdges: (t, ~options: mouseEdgesOptions=?, unit) => t = "mouseEdges";

  [@bs.send]
  external moveCenter: (t, ~x: float, ~y: float) => t = "moveCenter";

  [@bs.send]
  external moveCenterToPoint: (t, ~point: Point.t) => t = "moveCenter";

  [@bs.send]
  external pausePlugin: (t, ~type_: string) => unit = "pausePlugin";

  [@bs.send]
  external pinch: (t, ~options: pinchOptions=?, unit) => t = "pinch";

  [@bs.send]
  external removePlugin: (t, ~type_: string) => unit = "removePlugin";

  [@bs.send]
  external resize: (t, ~screenWidth:float=?, ~screenHeight:float=?, ~worldWidth:float=?, ~worldHeight:float=?, unit) => unit = "resize";

  [@bs.send]
  external resumePlugin: (t, ~type_: string) => unit = "resumePlugin";

  [@bs.send]
  external snap: (t, ~x: float, ~y: float, ~options: ([@bs.unwrap][`EaseString(snapOptionsEaseString) | `EaseFunction(snapOptionsEaseFunction)])=?, unit) => t = "snap";

  [@bs.send]
  external snapZoom: (t, ~options: ([@bs.unwrap][`EaseString(snapZoomOptionsEaseString) | `EaseFunction(snapZoomOptionsEaseFunction)])=?, unit) => t = "snapZoom";

  [@bs.send]
  external toScreen: (t, ~x: float, ~y: float) => Point.t = "toScreen";

  [@bs.send]
  external pointToScreen: (t, ~point: Point.t) => Point.t = "toScreen";

  [@bs.send]
  external toWorld: (t, ~x: float, ~y: float) => Point.t = "toWorld";

  [@bs.send]
  external pointToWorld: (t, ~point: Point.t) => Point.t = "toWorld";

  [@bs.send]
  external userPlugin: (t, ~name: string, ~plugin: Plugin.t, ~index: int) => unit = "userPlugin";

  [@bs.send]
  external wheel: (t, ~options: wheelOptions=?, unit) => t = "wheel";

  [@bs.send]
  external zoom: (t, ~change: float, ~center: bool=?, unit) => t = "zoom";

  [@bs.send]
  external zoomPercent: (t, ~percent: float, ~center: bool=?, unit) => t = "zoomPercent";
};