open Css;
open Operators;

module Styles {
  let root = style([
    media("(min-width: 960px)", [
      marginBottom(px(24))
    ])
  ]);

  let content = style([
    !#backgroundColor(hex("BD891A"))
  ]);

  let message = style([
    display(`flex), 
    alignItems(`center)
  ]);

  let messageText = style([
    marginLeft(px(16))
  ]);
};

type state = {
  open_: bool
};

type action = | SetOpen(bool);

let make = (~isOpen, ~message="", ~onExited, _children) => {
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
      className=Styles.root
      anchorOrigin=MaterialUi.Snackbar.AnchorOrigin.t(~horizontal=`Center, ~vertical=`Bottom)
      autoHideDuration=`Int(3000)
      resumeHideDuration=`Int(2000)
      open_=state.open_
      onClose=((_event, _reason) => send(SetOpen(false)))
      onExited>

      <MaterialUi.SnackbarContent 
        className=Styles.content
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