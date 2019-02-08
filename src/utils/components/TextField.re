open Operators;
open MaterialUi.TextField;
open Css;

module Styles {
  let inputLabelProps = [%bs.obj {
    classes: {
      root: style([
        fontFamily(Fonts.jost),
        textTransform(`uppercase),
        color(white)
      ] |> List.map(e => important(e)))
    }
  }];

  let inputProps = [%bs.obj {
    classes: {
      root: style([
        color(white) |> important
      ]),
      underline: style([
        borderBottom(px(1), `solid, white) |> important,
        selector("&:before", [
          borderBottom(px(0), `solid, white) |> important
        ]),
        selector("&:hover::before", [
          borderBottom(px(1), `solid, white) |> important
        ]),
        selector("&:after", [
          borderBottom(px(1), `solid, white) |> important
        ])
      ])
    }
  }];
};

let make = (
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
      _InputLabelProps=Styles.inputLabelProps 
      _InputProps=Styles.inputProps
      ?autoComplete ?autoFocus ?className ?defaultValue ?disabled ?error ?_FormHelperTextProps ?fullWidth ?helperText ?id ?inputProps ?inputRef ?label ?margin ?multiline ?name ?onBlur ?onChange ?onFocus ?placeholder ?required ?rows ?rowsMax ?select ?_SelectProps ?type_ ?value ?variant ?component
    >
      {children}
    </MaterialUi.TextField>
};