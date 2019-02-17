open Operators;
open MaterialUi.TextField;
open Css;

module Styles {
  let _rootInputLabelStyle = style([
    fontFamily(Fonts.jost),
    textTransform(`uppercase),
    color(rgba(255, 255, 255, 0.6))
  ] |> List.map(rule => !# rule));

  /* default margin is a bit too small so the label easily hits above objects */
  let baseMargin = style([
    !# marginTop(`px(8))
  ]);

  let inputLabelProps = [%bs.obj {
    classes: {
      root: _rootInputLabelStyle
    }
  }];

  let inputProps = [%bs.obj {
    classes: {
      root: style([
        !# color(white),
      ]),
      underline: style([
        !# borderBottom(px(1), `solid, rgba(255, 255, 255, 0.6)),
        selector("&:before", [
          !# borderBottom(px(0), `solid, white)
        ]),
        selector("&:hover::before", [
          !# borderBottom(px(1), `solid, white)
        ]),
        selector("&:after", [
          !# borderBottom(px(1), `solid, white)
        ])
      ])
    }
  }];
};

let make = (
  /* hides focused label on layouts with small phone breakpoint range */
  ~autoComplete: option(string)=?,
  ~autoFocus: option(bool)=?,
  ~className: option(string)=?,
  ~defaultValue:
    option([ | `String(string) | `Int(int) | `Float(float)])=?,
  ~disabled: option(bool)=?,
  ~error: option(bool)=?,
  ~_FormHelperTextProps: option(Js.t({..}))=?,
  ~fullWidth: option(bool)=?,
  ~helperText: option(ReasonReact.reactElement)=?,
  ~id: option(string)=?,
  ~_InputLabelProps: option(Js.t({..}))=?,
  ~_InputProps: option(Js.t({..}))=?,
  ~inputProps: option(Js.t({..}))=?,
  ~inputRef:
    option(
      [ | `Callback('genericCallback) | `ObjectGeneric(Js.t({..}))],
    )=?,
  ~label: option(ReasonReact.reactElement)=?,
  ~margin: option(margin)=?,
  ~multiline: option(bool)=?,
  ~name: option(string)=?,
  ~onBlur: option(ReactEvent.Focus.t => unit)=?,
  ~onChange: option(ReactEvent.Form.t => unit)=?,
  ~onFocus: option(ReactEvent.Focus.t => unit)=?,
  ~placeholder: option(string)=?,
  ~required: option(bool)=?,
  ~rows: option([ | `String(string) | `Int(int) | `Float(float)])=?,
  ~rowsMax: option([ | `String(string) | `Int(int) | `Float(float)])=?,
  ~select: option(bool)=?,
  ~_SelectProps: option(Js.t({..}))=?,
  ~type_: option(string)=?,
  ~value:
    option(
      [
        | `String(string)
        | `Int(int)
        | `Float(float)
        | `Bool(bool)
        | `Array(
            array(
              [
                | `String(string)
                | `Int(int)
                | `Float(float)
                | `Bool(bool)
              ],
            ),
          )
      ],
    )=?,
  ~variant: option(variant)=?,
  ~component:
    option(
      [
        | `String(string)
        | `Callback('genericCallback)
        | `ObjectGeneric(Js.t({..}))
      ],
    )=?,
  children
) => {
  ...ReasonReact.statelessComponent("TextField"),
  render: _self =>
    <MaterialUi.TextField
      className=([Styles.baseMargin, className |? ""] >|< " ")
      _InputLabelProps=Styles.inputLabelProps
      _InputProps=Styles.inputProps
      ?autoComplete ?autoFocus ?defaultValue ?disabled ?error ?_FormHelperTextProps ?fullWidth ?helperText ?id ?inputProps ?inputRef ?label ?margin ?multiline ?name ?onBlur ?onChange ?onFocus ?placeholder ?required ?rows ?rowsMax ?select ?_SelectProps ?type_ ?value ?variant ?component
    >
      {children}
    </MaterialUi.TextField>
};