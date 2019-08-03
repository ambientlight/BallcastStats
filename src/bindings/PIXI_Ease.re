open PIXI;

type easeFunction = (float, float, float, float) => float;
type stringOrEaseFunction;
type easeDisplayObjectOrEaseDisplayObjectArray;
type easeParams;
type addOptions;
type displayObjectOrPoint;
type stringOrStringArray;

[@bs.deriving abstract]
type easeOptionsEaseString = {
  [@bs.optional] duration: float,
  [@bs.optional] ease: string,
  [@bs.optional] useTicker: bool,
  [@bs.optional] ticker: Ticker.t,
  [@bs.optional] maxFrame: float
};

[@bs.deriving abstract]
type easeOptionsEaseFunction = {
  [@bs.optional] duration: float,
  [@bs.optional] ease: easeFunction,
  [@bs.optional] useTicker: bool,
  [@bs.optional] ticker: Ticker.t,
  [@bs.optional] maxFrame: float
};

[@bs.deriving abstract]
type easeParamsDisplayObject = {
  [@bs.optional] x: float,
  [@bs.optional] y: float,
  [@bs.optional] position: DisplayObject.t,
  [@bs.optional] width: float,
  [@bs.optional] height: float,
  [@bs.optional] scale: float,
  [@bs.optional] scaleX: float,
  [@bs.optional] scaleY: float,
  [@bs.optional] alpha: float,
  [@bs.optional] rotation: float,
  [@bs.optional] face: DisplayObject.t,
  [@bs.optional] skew: float,
  [@bs.optional] skewX: float,
  [@bs.optional] skewY: float,
  [@bs.optional] tint: array(int),
  [@bs.optional] blend: array(int),
  [@bs.optional] shake: float
};

[@bs.deriving abstract]
type easeParamsPoint = {
  [@bs.optional] x: float,
  [@bs.optional] y: float,
  [@bs.optional] position: Point.t,
  [@bs.optional] width: float,
  [@bs.optional] height: float,
  [@bs.optional] scale: float,
  [@bs.optional] scaleX: float,
  [@bs.optional] scaleY: float,
  [@bs.optional] alpha: float,
  [@bs.optional] rotation: float,
  [@bs.optional] face: Point.t,
  [@bs.optional] skew: float,
  [@bs.optional] skewX: float,
  [@bs.optional] skewY: float,
  [@bs.optional] tint: array(int),
  [@bs.optional] blend: array(int),
  [@bs.optional] shake: float
};

[@bs.deriving abstract]
type addOptionsEaseStringRepeatBool = {
  [@bs.optional] duration: float,
  [@bs.optional] ease: string,
  [@bs.optional] repeat: bool,
  [@bs.optional] reverse: bool,
  [@bs.optional] wait: int,
  [@bs.optional] removeExisting: bool
};

[@bs.deriving abstract]
type addOptionsEaseFunctionRepeatBool = {
  [@bs.optional] duration: float,
  [@bs.optional] ease: easeFunction,
  [@bs.optional] repeat: bool,
  [@bs.optional] reverse: bool,
  [@bs.optional] wait: int,
  [@bs.optional] removeExisting: bool
};

[@bs.deriving abstract]
type addOptionsEaseStringRepeatInt = {
  [@bs.optional] duration: float,
  [@bs.optional] ease: string,
  [@bs.optional] repeat: int,
  [@bs.optional] reverse: bool,
  [@bs.optional] wait: int,
  [@bs.optional] removeExisting: bool
};

[@bs.deriving abstract]
type addOptionsEaseFunctionRepeatInt = {
  [@bs.optional] duration: float,
  [@bs.optional] ease: easeFunction,
  [@bs.optional] repeat: int,
  [@bs.optional] reverse: bool,
  [@bs.optional] wait: int,
  [@bs.optional] removeExisting: bool
};

module EaseDisplayObject {
  class type _t = [@bs] {
    inherit EventEmitter._t;
  }

  type t = Js.t(_t);
};

module Easing {

  class type _t = [@bs] {
    inherit EventEmitter._t;
  };

  type t = Js.t(_t);

  [@bs.module "pixi-ease"][@bs.new]
  external create: (~element: [@bs.unwrap][`DisplayObject(DisplayObject.t) | `DisplayObjectArray(array(DisplayObject.t))], ~params: Js.t({..}), ~options: Js.t({..})) => t = "Easing";
};

class type _t = [@bs] {
  inherit EventEmitter._t;

  [@bs.set] pub count: int;
  [@bs.set] pub duration: float;
  [@bs.set] pub ease: stringOrEaseFunction;

  pub add: (DisplayObject.t, easeParams, Js.Undefined.t(addOptions)) => easeDisplayObjectOrEaseDisplayObjectArray;
  pub countRunning: unit => int;
  pub destroy: unit => unit;
  pub face: (DisplayObject.t, displayObjectOrPoint, int, Js.Undefined.t(addOptions)) => Easing.t;

  pub removeAll: unit => unit;
  pub removeAllEases: (DisplayObject.t) => unit;
  pub removeEase: (DisplayObject.t, stringOrStringArray) => unit;
  pub target: (DisplayObject.t, displayObjectOrPoint, int, Js.Undefined.t(addOptions)) => Easing.t;
  pub update: float => unit;
};

type t = Js.t(_t);

[@bs.module "pixi-ease"][@bs.new]
external create: (~options: ([@bs.unwrap][`EaseString(easeOptionsEaseString) | `EaseFunction(easeOptionsEaseFunction)])=?, unit) => t = "Ease";

[@bs.set]
external setEase: (t, [@bs.unwrap][`EaseString(string) | `EaseFunction(easeFunction)]) => unit = "ease";

[@bs.get]
external getEase: t => stringOrEaseFunction = "ease";

[@bs.get]
external getEaseString: t => string = "ease";

[@bs.get]
external getEaseFunction: t => easeFunction = "ease";

[@bs.send]
external _add: (t, 
  ~element: Js.t(#DisplayObject._t), 
  ~params: [@bs.unwrap][`DisplayObject(easeParamsDisplayObject) | `Point(easeParamsPoint)], 
  ~options: ([@bs.unwrap][
    | `EaseStringRepeatBool(addOptionsEaseStringRepeatBool) 
    | `EaseFunctionRepeatBool(addOptionsEaseFunctionRepeatBool)
    | `EaseStringRepeatInt(addOptionsEaseStringRepeatInt)
    | `EaseFunctionRepeatInt(addOptionsEaseFunctionRepeatInt)])=?, 
  unit) => easeDisplayObjectOrEaseDisplayObjectArray = "add";

let add = (ease: t, 
  ~element: Js.t(#DisplayObject._t), 
  ~params: [@bs.unwrap][`DisplayObject(easeParamsDisplayObject) | `Point(easeParamsPoint)],
  ~options: option([@bs.unwrap][
    | `EaseStringRepeatBool(addOptionsEaseStringRepeatBool) 
    | `EaseFunctionRepeatBool(addOptionsEaseFunctionRepeatBool)
    | `EaseStringRepeatInt(addOptionsEaseStringRepeatInt)
    | `EaseFunctionRepeatInt(addOptionsEaseFunctionRepeatInt)])=?, 
  unit): [`EaseDisplayObjectArray(array(EaseDisplayObject.t)) | `EaseDisplayObject(EaseDisplayObject.t)] => {
  
  module Internal {
    type constructor;
    [@bs.val] external array_: constructor = "Array";
    let instanceOf: ('a, constructor) => bool = [%bs.raw {|function(x,y) {return +(x instanceof y)}|}];
  };

  let res = _add(ease, ~element, ~params, ~options?, ());
  if(Internal.instanceOf(res, Internal.array_)){
    `EaseDisplayObjectArray(Obj.magic(res))
  } else {
    `EaseDisplayObject(Obj.magic(res))
  }
};

[@bs.send]
external countRunning: unit => int = "countRunning";

[@bs.send]
external destroy: t => unit = "destroy";

[@bs.send]
external face: (
  t, 
  ~element: DisplayObject.t, 
  ~target: [@bs.unwrap][`DisplayObject(DisplayObject.t) | `Point(Point.t)], 
  ~speed: int, 
  ~options: option([@bs.unwrap][
    | `EaseStringRepeatBool(addOptionsEaseStringRepeatBool) 
    | `EaseFunctionRepeatBool(addOptionsEaseFunctionRepeatBool)
    | `EaseStringRepeatInt(addOptionsEaseStringRepeatInt)
    | `EaseFunctionRepeatInt(addOptionsEaseFunctionRepeatInt)])=?,
  unit) => Easing.t = "face";

[@bs.send]
external removeAll: t => unit = "removeAll";

[@bs.send]
external removeAllEases: (t, ~object_: DisplayObject.t) => unit = "removeAllEases";

[@bs.send]
external removeEase: (t, ~element: DisplayObject.t, ~params: [@bs.unwrap][`String(string) | `StringArray(array(string))]) => unit = "removeEase";

[@bs.send]
external target: (
  t, 
  ~element: DisplayObject.t, 
  ~target: [@bs.unwrap][`DisplayObject(DisplayObject.t) | `Point(Point.t)],
  ~speed: int,
  ~options: option([@bs.unwrap][
    | `EaseStringRepeatBool(addOptionsEaseStringRepeatBool) 
    | `EaseFunctionRepeatBool(addOptionsEaseFunctionRepeatBool)
    | `EaseStringRepeatInt(addOptionsEaseStringRepeatInt)
    | `EaseFunctionRepeatInt(addOptionsEaseFunctionRepeatInt)])=?,
  unit) => Easing.t = "target";

[@bs.send]
external update: (t, ~elapsed: int) => unit = "update";

/*** default instantiated Ease class ***/
[@bs.module "pixi-ease"][@bs.val]
external ease: t = "ease";