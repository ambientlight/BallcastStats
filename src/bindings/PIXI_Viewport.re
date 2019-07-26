open PIXI;
open Webapi;

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

module LastViewport {
  class type _t = [@bs]{
    pub scaleX: float;
    pub scaleY: float;
    pub x: float;
    pub y: float;
  };

  type t = Js.t(_t);
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
  pub lastViewport: LastViewport.t;

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

[@bs.module][@bs.new]
external create: (~options: createOptions('a)=?, unit) => t = "pixi-viewport";

[@bs.send]
external bounce: (t, ~options: ([@bs.unwrap][`EaseString(bounceOptionsEaseString) | `EaseFunction(bounceOptionsEaseFunction)])=?, unit) => t = "bounce";

[@bs.send]
external clamp: (t, ~options: ([@bs.unwrap] [`FloatOptions(clampOptionsFloat) | `BoolOptions(clampOptionsBool)])=?, unit) => t = "clamp";

[@bs.send]
external clampZoom: (t, ~options: clampZoomOptions, unit) => t = "clampZoom";

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