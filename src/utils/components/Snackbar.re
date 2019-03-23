open Css;
open Operators;

module Styles {
  let root = style([
    media("(min-width: 960px)", [
      marginBottom(px(24))
    ])
  ]);

  let warning = style([
    !#backgroundColor(hex("BD891A"))
  ]);

  let success = style([
    !#backgroundColor(hex("226831"))
  ])

  let error = style([
    !#backgroundColor(hex("9c1c1c"))
  ])

  let message = style([
    display(`flex), 
    alignItems(`center)
  ]);

  let messageText = style([
    marginLeft(px(16))
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

let make = (~key=?, ~isOpen, ~message="", ~type_=Success, ~onExited, _children) => {
  ...ReasonReact.reducerComponent(__MODULE__),
  
  initialState: () => {
    open_: isOpen
  },

  reducer: (action, _state: state) =>
    switch(action){
    | SetOpen(open_) => ReasonReact.Update({ open_: open_ })
    },

  render: ({ state, send }) => 
    <MaterialUi.Snackbar
      ?key 
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
            <span className=Styles.messageText>{ReasonReact.string(message)}</span>
          </div>)
        
        action=(
          <MaterialUi.IconButton key="close" color=`Inherit onClick=(_event => send(SetOpen(false)))>
            <MaterialUiIcons.Close/>
          </MaterialUi.IconButton>
        )>
      </MaterialUi.SnackbarContent>
    </MaterialUi.Snackbar>
}