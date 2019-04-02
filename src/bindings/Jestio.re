type modifier('a) = [ | `Just('a) | `Not('a)];

let mapMod = f =>
  fun
  | `Just(a) => `Just(f(a))
  | `Not(a) => `Not(f(a));

type assertion =
  | Ok: assertion
  | Fail(string): assertion
  | ArrayContains(modifier((array('a), 'a))): assertion
  | ArrayLength(modifier((array('a), int))): assertion
  | ArraySuperset(modifier((array('a), array('a)))): assertion
  | Be(modifier(('a, 'a))): assertion
  | Equal(modifier(('a, 'a))): assertion
  | FloatCloseTo(modifier((float, float, option(int)))): assertion
  | GreaterThan(modifier(('a, 'a))): assertion
  | GreaterThanOrEqual(modifier(('a, 'a))): assertion
  | LessThan(modifier(('a, 'a))): assertion
  | LessThanOrEqual(modifier(('a, 'a))): assertion
  | StringContains(modifier((string, string))): assertion
  | StringMatch(modifier((string, Js.Re.t))): assertion
  | Throws(modifier(unit => _)): assertion
  | ThrowsException(modifier((unit => _, exn))): assertion
  | ThrowsMessage(modifier((unit => _, string))): assertion
  | ThrowsMessageRe(modifier((unit => _, Js.Re.t))): assertion
  | MatchSnapshot(_): assertion
  | MatchSnapshotName(_, string): assertion
  | ThrowsMatchSnapshot(unit => _): assertion
  /* JS */
  | Defined(modifier(Js.undefined('a))): assertion
  | Falsy(modifier('a)): assertion
  | Null(modifier(Js.null(_))): assertion
  | Truthy(modifier('a)): assertion
  | Undefined(modifier(Js.undefined('a))): assertion
  | ObjectContains(modifier((Js.t({..}), array(string)))): assertion
  | ObjectMatch(modifier((Js.t({..}), Js.t({..})))): assertion;

module type Asserter = {
  type t('a);
  let affirm: t('a) => unit;
};

/* internal */
module LLExpect: {
  type t('a) = assertion;
  let affirm: t('a) => unit;
} = {
  type t('a) = assertion;
  type specialMatch;

  [@bs.val] external expect: 'a => Js.t({..}) = "";
  [@bs.val] external fail: string => unit = "";
  [@bs.val]
  external arrayContaining: array('a) => specialMatch =
    "expect.arrayContaining";
  [@bs.val]
  external stringContaining: string => specialMatch =
    "expect.stringContaining";
  let objectContaining: array(string) => Js.t({..}) = [%raw
    {|
    function (properties) {
      var spec = {};
      properties.forEach(function (property) {
        spec[property] = expect.anything();
      });
      return spec;
    }
  |}
  ];

  let affirm =
    fun
    | Ok => ()
    | Fail(message) => fail(message)

    | ArrayContains(`Just(a, b)) => expect(a)##toContain(b)
    | ArrayContains(`Not(a, b)) => expect(a)##(!)##toContain(b)
    | ArrayLength(`Just(a, l)) => expect(a)##toHaveLength(l)
    | ArrayLength(`Not(a, l)) => expect(a)##(!)##toHaveLength(l)
    | ArraySuperset(`Just(a, b)) => expect(a)##toEqual(arrayContaining(b))
    | ArraySuperset(`Not(a, b)) =>
      expect(a)##(!)##toEqual(arrayContaining(b))
    | Be(`Just(a, b)) => expect(a)##toBe(b)
    | Be(`Not(a, b)) => expect(a)##(!)##toBe(b)
    | Equal(`Just(a, b)) => expect(a)##toEqual(b)
    | Equal(`Not(a, b)) => expect(a)##(!)##toEqual(b)
    | FloatCloseTo(`Just(a, b, p)) =>
      expect(a)##toBeCloseTo(b, Js.Undefined.fromOption(p))
    | FloatCloseTo(`Not(a, b, p)) =>
      expect(a)##(!)##toBeCloseTo(b, Js.Undefined.fromOption(p))
    | GreaterThan(`Just(a, b)) => expect(a)##toBeGreaterThan(b)
    | GreaterThan(`Not(a, b)) => expect(a)##(!)##toBeGreaterThan(b)
    | GreaterThanOrEqual(`Just(a, b)) =>
      expect(a)##toBeGreaterThanOrEqual(b)
    | GreaterThanOrEqual(`Not(a, b)) =>
      expect(a)##(!)##toBeGreaterThanOrEqual(b)
    | LessThan(`Just(a, b)) => expect(a)##toBeLessThan(b)
    | LessThan(`Not(a, b)) => expect(a)##(!)##toBeLessThan(b)
    | LessThanOrEqual(`Just(a, b)) => expect(a)##toBeLessThanOrEqual(b)
    | LessThanOrEqual(`Not(a, b)) => expect(a)##(!)##toBeLessThanOrEqual(b)
    | StringMatch(`Just(s, re)) => expect(s)##toMatch(re)
    | StringMatch(`Not(s, re)) => expect(s)##(!)##toMatch(re)
    | StringContains(`Just(a, b)) =>
      expect(a)##toEqual(stringContaining(b))
    | StringContains(`Not(a, b)) =>
      expect(a)##(!)##toEqual(stringContaining(b))

    | Throws(`Just(f)) => expect(f)##toThrow()
    | Throws(`Not(f)) => expect(f)##(!)##toThrow()
    | ThrowsException(`Just(f, e)) =>
      expect(f)##toThrow(Js.String.make(e))
    | ThrowsException(`Not(f, e)) =>
      expect(f)##(!)##toThrow(Js.String.make(e))
    | ThrowsMessage(`Just(f, msg)) => expect(f)##toThrow(msg)
    | ThrowsMessage(`Not(f, msg)) => expect(f)##(!)##toThrow(msg)
    | ThrowsMessageRe(`Just(f, re)) => expect(f)##toThrow(re)
    | ThrowsMessageRe(`Not(f, re)) => expect(f)##(!)##toThrow(re)

    | MatchSnapshot(a) => expect(a)##toMatchSnapshot()
    | [@implicit_arity] MatchSnapshotName(a, name) =>
      expect(a)##toMatchSnapshot(name)
    | ThrowsMatchSnapshot(f) => expect(f)##toThrowErrorMatchingSnapshot()

    /* JS */
    | Defined(`Just(a)) => expect(a)##toBeDefined()
    | Defined(`Not(a)) => expect(a)##(!)##toBeDefined()
    | Falsy(`Just(a)) => expect(a)##toBeFalsy()
    | Falsy(`Not(a)) => expect(a)##(!)##toBeFalsy()
    | Null(`Just(a)) => expect(a)##toBeNull()
    | Null(`Not(a)) => expect(a)##(!)##toBeNull()
    | Truthy(`Just(a)) => expect(a)##toBeTruthy()
    | Truthy(`Not(a)) => expect(a)##(!)##toBeTruthy()
    | Undefined(`Just(a)) => expect(a)##toBeUndefined()
    | Undefined(`Not(a)) => expect(a)##(!)##toBeUndefined()
    | ObjectContains(`Just(a, props)) =>
      expect(a)##toEqual(objectContaining(props))
    | ObjectContains(`Not(a, props)) =>
      expect(a)##(!)##toEqual(objectContaining(props))
    | ObjectMatch(`Just(a, b)) => expect(a)##toMatchObject(b)
    | ObjectMatch(`Not(a, b)) => expect(a)##(!)##toMatchObject(b);
};

module Runner = (A: Asserter) => {
  let affirm = A.affirm;
  [@bs.val]
  external _test: (string, [@bs.uncurry] (unit => Js.undefined(unit))) => unit =
    "test";
  [@bs.val]
  external _testAsync:
    (
      string,
      ((. unit) => unit) => Js.undefined(unit),
      Js.Undefined.t(int)
    ) =>
    unit =
    "test";
  [@bs.val]
  external _testPromise:
    (
      string,
      [@bs.uncurry] (unit => Js.Promise.t('a)),
      Js.Undefined.t(int)
    ) =>
    unit =
    "test";

  let test = (name, callback) =>
    _test(
      name,
      () => {
        affirm @@ callback();
        Js.undefined;
      },
    );

  let testAsync = (name, ~timeout=?, callback) =>
    _testAsync(
      name,
      finish => {
        callback(case => {
          affirm(case);
          finish(.);
        });
        Js.undefined;
      },
      Js.Undefined.fromOption(timeout),
    );

  let testPromise = (name, ~timeout=?, callback) =>
    _testPromise(
      name,
      () =>
        callback()
        |> Js.Promise.then_(a => a |> A.affirm |> Js.Promise.resolve),
      Js.Undefined.fromOption(timeout),
    );

  let testAll = (name, inputs, callback) =>
    inputs
    |> List.iter(input => {
         let name = {j|$name - $input|j};
         _test(
           name,
           () => {
             affirm @@ callback(input);
             Js.undefined;
           },
         );
       });

  [@bs.val]
  external describe:
    (string, [@bs.uncurry] (unit => Js.undefined(unit))) => unit =
    "";
  let describe = (label, f) =>
    describe(
      label,
      () => {
        f();
        Js.undefined;
      },
    );

  [@bs.val] external beforeAll: ([@bs.uncurry] (unit => unit)) => unit = "";
  [@bs.val]
  external beforeAllAsync:
    (((. unit) => unit) => Js.undefined(unit), Js.Undefined.t(int)) => unit =
    "beforeAll";
  let beforeAllAsync = (~timeout=?, callback) =>
    beforeAllAsync(
      finish => {
        callback(() => finish(.));
        Js.undefined;
      },
      Js.Undefined.fromOption(timeout),
    );
  [@bs.val]
  external beforeAllPromise:
    ([@bs.uncurry] (unit => Js.Promise.t('a)), Js.Undefined.t(int)) => unit =
    "beforeAll";
  let beforeAllPromise = (~timeout=?, callback) =>
    beforeAllPromise(
      () => callback() |> Js.Promise.resolve,
      Js.Undefined.fromOption(timeout),
    );

  [@bs.val] external beforeEach: ([@bs.uncurry] (unit => unit)) => unit = "";
  [@bs.val]
  external beforeEachAsync:
    (((. unit) => unit) => Js.undefined(unit), Js.Undefined.t(int)) => unit =
    "beforeEach";
  let beforeEachAsync = (~timeout=?, callback) =>
    beforeEachAsync(
      finish => {
        callback(() => finish(.));
        Js.undefined;
      },
      Js.Undefined.fromOption(timeout),
    );
  [@bs.val]
  external beforeEachPromise:
    ([@bs.uncurry] (unit => Js.Promise.t('a)), Js.Undefined.t(int)) => unit =
    "beforeEach";
  let beforeEachPromise = (~timeout=?, callback) =>
    beforeEachPromise(
      () => callback() |> Js.Promise.resolve,
      Js.Undefined.fromOption(timeout),
    );

  [@bs.val] external afterAll: ([@bs.uncurry] (unit => unit)) => unit = "";
  [@bs.val]
  external afterAllAsync:
    (((. unit) => unit) => Js.undefined(unit), Js.Undefined.t(int)) => unit =
    "afterAll";
  let afterAllAsync = (~timeout=?, callback) =>
    afterAllAsync(
      finish => {
        callback(() => finish(.));
        Js.undefined;
      },
      Js.Undefined.fromOption(timeout),
    );
  [@bs.val]
  external afterAllPromise:
    ([@bs.uncurry] (unit => Js.Promise.t('a)), Js.Undefined.t(int)) => unit =
    "afterAll";
  let afterAllPromise = (~timeout=?, callback) =>
    afterAllPromise(
      () => callback() |> Js.Promise.resolve,
      Js.Undefined.fromOption(timeout),
    );

  [@bs.val] external afterEach: ([@bs.uncurry] (unit => unit)) => unit = "";
  [@bs.val]
  external afterEachAsync:
    (((. unit) => unit) => Js.undefined(unit), Js.Undefined.t(int)) => unit =
    "afterEach";
  let afterEachAsync = (~timeout=?, callback) =>
    afterEachAsync(
      finish => {
        callback(() => finish(.));
        Js.undefined;
      },
      Js.Undefined.fromOption(timeout),
    );
  [@bs.val]
  external afterEachPromise:
    ([@bs.uncurry] (unit => Js.Promise.t('a)), Js.Undefined.t(int)) => unit =
    "afterEach";
  let afterEachPromise = (~timeout=?, callback) =>
    afterEachPromise(
      () => callback() |> Js.Promise.resolve,
      Js.Undefined.fromOption(timeout),
    );

  module Only = {
    [@bs.val]
    external _test:
      (string, [@bs.uncurry] (unit => Js.undefined(unit))) => unit =
      "it.only";
    [@bs.val]
    external _testAsync:
      (
        string,
        ((. unit) => unit) => Js.undefined(unit),
        Js.Undefined.t(int)
      ) =>
      unit =
      "it.only";
    [@bs.val]
    external _testPromise:
      (
        string,
        [@bs.uncurry] (unit => Js.Promise.t('a)),
        Js.Undefined.t(int)
      ) =>
      unit =
      "it.only";

    let test = (name, callback) =>
      _test(
        name,
        () => {
          affirm @@ callback();
          Js.undefined;
        },
      );

    let testAsync = (name, ~timeout=?, callback) =>
      _testAsync(
        name,
        finish => {
          callback(assertion => {
            affirm(assertion);
            finish(.);
          });
          Js.undefined;
        },
        Js.Undefined.fromOption(timeout),
      );

    let testPromise = (name, ~timeout=?, callback) =>
      _testPromise(
        name,
        () =>
          callback()
          |> Js.Promise.then_(a => a |> affirm |> Js.Promise.resolve),
        Js.Undefined.fromOption(timeout),
      );

    let testAll = (name, inputs, callback) =>
      inputs
      |> List.iter(input => {
           let name = {j|$name - $input|j};
           _test(
             name,
             () => {
               affirm @@ callback(input);
               Js.undefined;
             },
           );
         });

    [@bs.val]
    external describe:
      (string, [@bs.uncurry] (unit => Js.undefined(unit))) => unit =
      "describe.only";
    let describe = (label, f) =>
      describe(
        label,
        () => {
          f();
          Js.undefined;
        },
      );
  };

  module Skip = {
    [@bs.val]
    external test: (string, [@bs.uncurry] (unit => A.t('a))) => unit =
      "it.skip";
    [@bs.val]
    external testAsync: (string, (A.t('a) => unit) => unit) => unit =
      "it.skip";
    let testAsync = (name, ~timeout as _=?, callback) =>
      testAsync(name, callback);
    [@bs.val]
    external testPromise:
      (string, [@bs.uncurry] (unit => Js.Promise.t(A.t('a)))) => unit =
      "it.skip";
    let testPromise = (name, ~timeout as _=?, callback) =>
      testPromise(name, callback);
    let testAll = (name, inputs, callback) =>
      inputs
      |> List.iter(input => {
           let name = {j|$name - $input|j};
           test(name, () =>
             callback(input)
           );
         });
    [@bs.val]
    external describe:
      (string, [@bs.uncurry] (unit => Js.undefined(unit))) => unit =
      "describe.skip";
    let describe = (label, f) =>
      describe(
        label,
        () => {
          f();
          Js.undefined;
        },
      );
  };
};

include Runner(LLExpect);

let pass = Ok;
let fail = message => Fail(message);
/*
 * Not implemented:
 * - expect.anything - pointless when there's `option`, `Js.null` etc.
 * - expect.any - pointless when you have types, except against < .. > Js.t, but how to implement this?
 * - expect.arrayContaining - implement as overloads of `toEqual`, `toBeCalledWith`, `objectContaining` and `toMatchObject`
 * - expect.assertions - Not supported. There should be only one assertion per test.
 * - expect.objectContaining - implement as separate matcher and overload of `toBeCalledWith`
 * - expect.stringContaining - implement as overloads of `toEqual`, `toBeCalledWith`, `objectContaining` and `toMatchObject`
 * - expect.stringMatching - implement as overloads of `toEqual`, `toBeCalledWith`, `objectContaining` and `toMatchObject`
 */

module Expect = {
  type plainPartial('a) = [ | `Just('a)];
  type invertedPartial('a) = [ | `Not('a)];
  type partial('a) = modifier('a);

  let expect = a => `Just(a);

  let expectFn = (f, a) => `Just(() => f(a));

  let toBe = (b, p) => Be(mapMod(a => (a, b), p));

  /* toHaveBeenCalled* */

  let toBeCloseTo = (b, p) => FloatCloseTo(mapMod(a => (a, b, None), p));

  let toBeSoCloseTo = (b, ~digits, p) =>
    FloatCloseTo(mapMod(a => (a, b, Some(digits)), p));

  let toBeGreaterThan = (b, p) => GreaterThan(mapMod(a => (a, b), p));

  let toBeGreaterThanOrEqual = (b, p) =>
    GreaterThanOrEqual(mapMod(a => (a, b), p));

  let toBeLessThan = (b, p) => LessThan(mapMod(a => (a, b), p));

  let toBeLessThanOrEqual = (b, p) =>
    LessThanOrEqual(mapMod(a => (a, b), p));

  /** replaces expect.arrayContaining */

  let toBeSupersetOf = (b, p) => ArraySuperset(mapMod(a => (a, b), p));

  let toContain = (b, p) => ArrayContains(mapMod(a => (a, b), p));

  /** replaces expect.stringContaining */

  let toContainString = (b, p) => StringContains(mapMod(a => (a, b), p));

  let toEqual = (b, p) => Equal(mapMod(a => (a, b), p));

  let toHaveLength = (l, p) => ArrayLength(mapMod(a => (a, l), p));

  let toMatch = (s, p) =>
    StringMatch(mapMod(a => (a, Js.Re.fromString(s)), p));

  let toMatchRe = (re, p) => StringMatch(mapMod(a => (a, re), p));

  let toMatchSnapshot = (`Just(a)) => MatchSnapshot(a);

  let toMatchSnapshotWithName = (name, `Just(a)) =>
    [@implicit_arity] MatchSnapshotName(a, name);

  let toThrow = f => Throws((f :> modifier(_)));

  let toThrowErrorMatchingSnapshot = (`Just(f)) => ThrowsMatchSnapshot(f);

  let toThrowException = (e, p) => ThrowsException(mapMod(f => (f, e), p));

  let toThrowMessage = (message, p) =>
    ThrowsMessage(mapMod(f => (f, message), p));

  let toThrowMessageRe = (re, p) =>
    ThrowsMessageRe(mapMod(f => (f, re), p));

  let not = (`Just(a)) => `Not(a);
  let not_ = not; /* For Reason syntax compatibility. TODO: deprecate and remove */

  module Operators = {
    /** experimental */;

    let (===) = (a, b) => toBe(b, a);
    let (>) = (a, b) => toBeGreaterThan(b, a);
    let (>=) = (a, b) => toBeGreaterThanOrEqual(b, a);
    let (<) = (a, b) => toBeLessThan(b, a);
    let (<=) = (a, b) => toBeLessThanOrEqual(b, a);
    let (==) = (a, b) => toEqual(b, a);
    let (!=) = (a, b) => a |> not |> toEqual(b);
    let (!==) = (a, b) => a |> not |> toBe(b);
  };
};

module ExpectJs = {
  include Expect;

  let toBeDefined = a => Defined((a :> modifier(_)));
  let toBeFalsy = a => Falsy((a :> modifier(_)));
  /* toBeInstanceOf */
  let toBeNull = a => Null((a :> modifier(_)));
  let toBeTruthy = a => Truthy((a :> modifier(_)));
  let toBeUndefined = a => Undefined((a :> modifier(_)));

  /** replaces expect.objectContaining */

  let toContainProperties = (props, p) =>
    ObjectContains(mapMod(a => (a, props), p));

  let toMatchObject = (b, p) => ObjectMatch(mapMod(a => (a, b), p));
};

module MockJs = {
  /** experimental */;

  type fn('fn, 'args, 'ret);

  %bs.raw
  {|
    function makeNewMock(self) {
      return new (Function.prototype.bind.apply(self, arguments));
    }
  |};

  [@bs.deriving jsConverter]
  type resultType = [
    | [@bs.as "return"] `Return
    | [@bs.as "throw"] `Throw
    | [@bs.as "incomplete"] `Incomplete
  ];

  [@bs.deriving abstract]
  type result('a) = {
    [@bs.as "type"] type_: string,
    value: 'a
  };

  [@bs.val]
  external new0: fn(unit => 'ret, unit, 'ret) => 'ret = "makeNewMock";
  let new0 = new0;
  [@bs.val]
  external new1: (fn('a => 'ret, 'a, 'ret), 'a) => 'ret = "makeNewMock";
  let new1 = (a, self) => new1(self, a);
  [@bs.val]
  external new2: (fn((. 'a, 'b) => 'ret, ('a, 'b), 'ret), 'a, 'b) => 'ret =
    "makeNewMock";
  let new2 = (a, b, self) => new2(self, a, b);

  type mockConstructor;

  external fn: fn('fn, _, _) => 'fn = "%identity";
  
  [@bs.get] [@bs.scope "mock"]
  external calls: fn(_, 'args, _) => array('args) = "";
  let calls = self => Js.Array.copy(calls(self)); /* Awesome, the bloody things are mutated so we need to copy */
  let calls = self =>
    calls(self)
    |> Array.map([%bs.raw {| function (args) { return args.length === 1 ? args[0] : args }|}]); /* there's no such thing as aa 1-ary tuple, so we need to unbox single-element arrays */

  [@bs.get] [@bs.scope "mock"]
  external results: fn(_, _, 'ret) => array(result('ret)) = "";
  let results = self => Js.Array.copy(results(self)); /* Awesome, the bloody things are mutated so we need to copy */
  let results = self =>
    results(self)
    |> Array.map([%bs.raw {| function (args) { return args.length === 1 ? args[0] : args }|}]); /* there's no such thing as aa 1-ary tuple, so we need to unbox single-element arrays */

  [@bs.get] [@bs.scope "mock"]
  external instances: fn(_, _, 'ret) => array('ret) = ""; /* TODO: semms this only records "instances" created by `new` */
  let instances = self => Js.Array.copy(instances(self)); /* Awesome, the bloody things are mutated so we need to copy */

  /** Beware: this actually replaces `mock`, not just `mock.instances` and `mock.calls` */
  [@bs.send.pipe: fn(_)]
  external mockClear: unit = "";
  [@bs.send.pipe: fn(_)] external mockReset: unit = "";
  [@bs.send.pipe: fn('fn, _, _) as 'self]
  external mockImplementation: 'fn => 'self = "";
  [@bs.send.pipe: fn('fn, _, _) as 'self]
  external mockImplementationOnce: 'fn => 'self = "";
  [@bs.send.pipe: fn(_, _, 'ret)] external mockReturnThis: unit = ""; /* not type safe, we don't know what `this` actually is */
  [@bs.send.pipe: fn(_, _, 'ret) as 'self]
  external mockReturnValue: 'ret => 'self = "";
  [@bs.send.pipe: fn(_, _, 'ret) as 'self]
  external mockReturnValueOnce: 'ret => 'self = "";
 
  [@bs.send.pipe: fn('a, 'b, 'ret) as 'self]
  external mockResolvedValue: 'ret => fn(_ => Js.Promise.t('ret), 'b, Js.Promise.t('ret)) = "";
  [@bs.send.pipe: fn('a, 'b, 'ret) as 'self]
  external mockResolvedValueOnce: 'ret => fn(_ => Js.Promise.t('ret), 'b, Js.Promise.t('ret)) = "";

  [@bs.send] external call: ((. 'a) => 'b, 'c, 'a) => 'b = "";
  let call = (self, arg) => call(self |> Obj.magic, (), arg);
};

module Jest = {
  [@bs.val] external clearAllTimers: unit => unit = "jest.clearAllTimers";
  [@bs.val] external runAllTicks: unit => unit = "jest.runAllTicks";
  [@bs.val] external runAllTimers: unit => unit = "jest.runAllTimers";
  [@bs.val] external runAllImmediates: unit => unit = "jest.runAllImmediates";
  [@bs.val] external runTimersToTime: int => unit = "jest.runTimersToTime";
  [@bs.val]
  external advanceTimersByTime: int => unit = "jest.advanceTimersByTime";
  [@bs.val]
  external runOnlyPendingTimers: unit => unit = "jest.runOnlyPendingTimers";
  [@bs.val] external useFakeTimers: unit => unit = "jest.useFakeTimers";
  [@bs.val] external useRealTimers: unit => unit = "jest.useRealTimers";
};

module JestJs = {
  /** experimental */;

  [@bs.val] external disableAutomock: unit => unit = "jest.disableAutomock";
  [@bs.val] external enableAutomock: unit => unit = "jest.enableAutomock";
  /* genMockFromModule */
  [@bs.val] external resetModules: unit => unit = "jest.resetModules";
  [@bs.val]
  external inferred_fn:
    unit => MockJs.fn((. 'a) => Js.undefined('b), 'a, Js.undefined('b)) =
    "jest.fn"; /* not sure how useful this really is */
  [@bs.val]
  external fn: ('a => 'b) => MockJs.fn('a => 'b, 'a, 'b) = "jest.fn";
  [@bs.val]
  external fn2:
    ((. 'a, 'b) => 'c) => MockJs.fn((. 'a, 'b) => 'c, ('a, 'b), 'c) =
    "jest.fn" /* external isMockFunction : MockJs.fn -> Js.boolean = "jest.isMockFunction" [@@bs.val] */; /* pointless with types? */
  /* TODO
     external fn3 : ('a -> 'b -> 'c -> 'd) -> ('a * 'b * 'c) MockJs.fn = "jest.fn" [@@bs.val]
     external fn4 : ('a -> 'b -> 'c -> 'd -> 'e) -> ('a * 'b * 'c * 'd) MockJs.fn = "jest.fn" [@@bs.val]
     external fn5 : ('a -> 'b -> 'c -> 'd -> 'e -> 'f) -> ('a * 'a * 'c * 'd * 'e) MockJs.fn = "jest.fn" [@@bs.val]
     external fn6 : ('a -> 'b -> 'c -> 'd -> 'e -> 'f -> 'g) -> ('a * 'b * 'c * 'd * 'e * 'f) MockJs.fn = "jest.fn" [@@bs.val]
     */
  [@bs.val] external mock: string => unit = "jest.mock";
  [@bs.val]
  external mockWithFactory: (string, unit => 'a) => unit = "jest.mock";
  [@bs.val]
  external mockVirtual: (string, unit => 'a, Js.t({..})) => unit =
    "jest.mock";
  /* TODO If this is merely defined, babel-plugin-jest-hoist fails with "The second argument of `jest.mock` must be a function." Silly thing.
     let mockVirtual : string -> (unit -> 'a) -> unit =
       fun moduleName factory -> mockVirtual moduleName factory [%bs.obj { _virtual = Js.true_ }]
     */
  [@bs.val] external clearAllMocks: unit => unit = "jest.clearAllMocks";
  [@bs.val] external resetAllMocks: unit => unit = "jest.resetAllMocks";
  [@bs.val] external setMock: (string, Js.t({..})) => unit = "jest.setMock";
  [@bs.val] external unmock: string => unit = "jest.unmock";
  [@bs.val]
  external spyOn:
    (Js.t({..}) as 'this, string) => MockJs.fn(unit, unit, 'this) =
    "jest.spyOn"; /* this is a bit too dynamic */

  /* opaque type to be used in module bindings for mock functions assignments 
    [@bs.module "aws-amplify"] external auth: moduleType = "Auth";
  */
  type moduleType;
  let _assignMock: (. moduleType, string, Js.t({.})) => unit =
    [%raw "function (module, key, value) { module[key] = value; }"];
  
  let assignMock = (jsModule, key, mockfn) => _assignMock(. jsModule, key, mockfn |> Obj.magic)
};
