type t;

[@bs.val][@bs.scope "Object"]
external _defineSymbol: ('a, t, 'b) => unit = "defineProperty"; 

let create: string => t = [%raw {|
  function(key){ return Symbol(key) }
|}];

let getValue = (obj, symbol: t) => Js.Dict.unsafeGet(Obj.magic(obj), Obj.magic(symbol));
let setValue = (obj, symbol, value) => _defineSymbol(obj, symbol, [%bs.obj {
  value: value,
  writable: false
}]);

[@bs.val][@bs.scope "Object"]
external _getOwnPropertySymbols: 'a => array(t) = "getOwnPropertySymbols";

let getOwnPropertySymbols = obj => switch(Js.Types.classify(obj)){
  | JSObject(obj) => _getOwnPropertySymbols(obj)
  | _ => [||]
};

module Debug {
  [@bs.deriving jsConverter]
  type t = [
    | [@bs.as "Symbol(BsVariant)"] `BsVariant
    | [@bs.as "Symbol(BsPolyVar)"] `BsPolyVar
    | [@bs.as "Symbol(BsRecord)"] `BsRecord
    | [@bs.as "Symbol(ReductiveDevToolsBsLabeledVariant)"] `DevToolsBsLabeledVariant
    | [@bs.as "Symbol(ReductiveDevToolsDontSerialize)"] `DevToolsDontSerialize
  ];

  let ofReasonAction = action => {
    let symbols = getOwnPropertySymbols(action);
    let extractedSymbols = symbols
      |> Array.mapi((idx, symbol) => (idx, {j|$symbol|j}));

    /** 
     * make sure Bs* symbols always appearing before ReductiveDevTool ones 
     * (logic will pick first symbol available)
     * assumption: Bs* symbols are exclusive, but they can combine with ReductiveDevTool* ones
     * which provide additional metadata(user specified)
     */
    Array.sort(((_, lhs), (_, rhs)) => compare(lhs, rhs), extractedSymbols);
    extractedSymbols
      |> Array.map(((idx, symbol)) => (idx, tFromJs(symbol)))
      |. Belt.Array.keep(((_, symbol)) => Belt.Option.isSome(symbol))
      |> Array.map( ((idx, symbol)) => (idx, Belt.Option.getExn(symbol)) );
    }

  let symbolValue = (action, debugSymbol: t) => 
    ofReasonAction(action)
    |. Belt.Array.keep(((_, symbol)) => symbol == debugSymbol)
    |. Belt.Array.get(0)
    |. Belt.Option.map(((idx, _)) => {
      let symbol = Array.get(getOwnPropertySymbols(action), idx);
      getValue(action, symbol)
    })
};