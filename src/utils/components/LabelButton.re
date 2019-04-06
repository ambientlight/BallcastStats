open Operators;
open MaterialUi.TextField;
open Css;

/* mimic typography colors */
type color = [
  | `Default
  | `Error
  | `Inherit
  | `Primary
  | `Secondary
  | `TextPrimary
  | `TextSecondary
];

module Styles {  
  let base = style([
    selector("&:hover", [
      textDecoration(`underline),
      cursor(`pointer),
      opacity(1.0)
    ])
  ]);

  let emphasized = style([opacity(1.0)])
  let normal = style([opacity(0.65)])

  let inherit_ = style([color(raw("inherit"))]);
  let error = MaterialUi.Theme.(style([
    color(raw(AppTheme.theme|.Theme.paletteGet|.Palette.errorGet|.PaletteColor.mainGet))
  ]));
  let primary = MaterialUi.Theme.(style([
    color(raw(AppTheme.theme|.Theme.paletteGet|.Palette.primaryGet|.PaletteColor.mainGet))
  ]));
  let secondary = MaterialUi.Theme.(style([
    color(raw(AppTheme.theme|.Theme.paletteGet|.Palette.secondaryGet|.PaletteColor.mainGet))
  ]));
  let textPrimary = MaterialUi.Theme.(style([
    color(raw(AppTheme.theme|.Theme.paletteGet|.Palette.textGet|.TypeText.primaryGet))
  ]));
  let textSecondary = MaterialUi.Theme.(style([
    color(raw(AppTheme.theme|.Theme.paletteGet|.Palette.textGet|.TypeText.secondaryGet))
  ]));
}

let make = (~onClick: option(ReactEvent.Mouse.t => unit)=?, ~emphasized=false, ~color: option(color)=?, ~className: option(string)=?, children) => {
  ...ReasonReact.statelessComponent(__MODULE__),
  render: _self =>
    <span
    className=([
      Styles.base, 
      emphasized ? Styles.emphasized : Styles.normal,
      (switch(color |? `Default){
      | `Error => Styles.error
      | `Inherit => Styles.inherit_
      | `Primary => Styles.primary
      | `Secondary => Styles.secondary
      | `TextPrimary => Styles.textPrimary
      | `TextSecondary => Styles.textSecondary
      | _ => ""
      }),
      className |? "",
    ] >|< " ")
    ?onClick>
      ...children
    </span>
}
  
