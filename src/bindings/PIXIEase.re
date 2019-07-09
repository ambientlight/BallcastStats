open PIXI;

type easeFunction = (float, float, float, float) => float;
type stringOrEaseFunction;
type easeDisplayObjectOrEaseDisplayObjectArray;

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

module Ease {
  
  class type _t = [@bs] {
    inherit EventEmitter._t;

    [@bs.set] pub duration: float;
    [@bs.set] pub ease: stringOrEaseFunction;

    pub destroy: unit => unit;
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
  external destroy: t => unit = "destroy";

  [@bs.send]
  external _add: (t, 
    ~element: DisplayObject.t, 
    ~params: [@bs.unwrap][`DisplayObject(easeParamsDisplayObject) | `Point(easeParamsPoint)], 
    ~options: ([@bs.unwrap][
      | `EaseStringRepeatBool(addOptionsEaseStringRepeatBool) 
      | `EaseFunctionRepeatBool(addOptionsEaseFunctionRepeatBool)
      | `EaseStringRepeatInt(addOptionsEaseStringRepeatInt)
      | `EaseFunctionRepeatInt(addOptionsEaseFunctionRepeatInt)])=?, 
    unit) => easeDisplayObjectOrEaseDisplayObjectArray = "add";

  let add = (ease: t, 
    ~element: DisplayObject.t, 
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
}