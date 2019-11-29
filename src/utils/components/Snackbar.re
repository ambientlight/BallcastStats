open Css;
open Operators;

module Styles {
  let root = style([
    Media.atLeast(Media.Breakpoint.Laptop, [
      marginBottom(px(24))
    ])
  ]);

  let warning = MaterialUi.Theme.(style([
    !#backgroundColor(
      raw(AppTheme.theme|.Theme.paletteGet|.Palette.secondaryGet|.PaletteColor.lightGet),
    ),
    !#color(
      raw(AppTheme.theme|.Theme.paletteGet|.Palette.secondaryGet|.PaletteColor.contrastTextGet)
    )
  ]));

  let success = MaterialUi.Theme.(style([
    !#backgroundColor(
      raw(AppTheme.theme|.Theme.paletteGet|.Palette.primaryGet|.PaletteColor.lightGet)
    ),
    !#color(
      raw(AppTheme.theme|.Theme.paletteGet|.Palette.primaryGet|.PaletteColor.contrastTextGet)
    )
  ]));

  let error = MaterialUi.Theme.(style([
    !#backgroundColor(
      raw(AppTheme.theme|.Theme.paletteGet|.Palette.errorGet|.PaletteColor.mainGet)
    ),
    !#color(
      raw(AppTheme.theme|.Theme.paletteGet|.Palette.errorGet|.PaletteColor.contrastTextGet)
    )
  ]));

  let message = style([
    display(`flex), 
    alignItems(`center)
  ]);

  let messageText = style([
    !#marginLeft(px(16))
  ]);
};

type notificationType = 
  | Success
  | Warning 
  | Error;

type state = {
  open_: bool
};

type action = | SetOpen(bool);

[@react.component]
let make = (~isOpen, ~message="", ~type_=Success, ~onExited) => {
  let (state, send) = React.useReducer(
    (_state: state, action) =>
      switch(action){
      | SetOpen(open_) => { open_: open_ }
      },
    {
      open_: isOpen
    }
  );

  <MaterialUi.Snackbar
    className=Styles.root
    anchorOrigin=MaterialUi.Snackbar.AnchorOrigin.t(~horizontal=`Center, ~vertical=`Bottom)
    autoHideDuration=`Int(5000)
    open_=state.open_
    onClose=((_event, _reason) => send(SetOpen(false)))
    onExited>

    <MaterialUi.SnackbarContent 
      className=(switch(type_){
      | Success => Styles.success
      | Warning => Styles.warning
      | Error => Styles.error
      })
      message=(
        <div className=Styles.message>
          <MaterialUiIcons.Warning/>
          <MaterialUi.Typography color=`Inherit className=Styles.messageText>{ReasonReact.string(message)}</MaterialUi.Typography>
        </div>)
      
      action=(
        <MaterialUi.IconButton color=`Inherit key="close" onClick=(_event => send(SetOpen(false)))>
          <MaterialUiIcons.Close/>
        </MaterialUi.IconButton>
      )>
    </MaterialUi.SnackbarContent>
  </MaterialUi.Snackbar>
};