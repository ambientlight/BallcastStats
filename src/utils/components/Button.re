open List;
open MaterialUi.Button;
open Operators;

module type StyleRef = {
  let buttonStyle: string;
  let raised: bool;
};

module Build = (Style: StyleRef) => {
  let make = (
    ~className: option(string)=?,
    ~color: option(color)=?,
    ~component: option([
      | `String(string)
      | `Callback('genericCallback)
      | `ObjectGeneric(Js.t({..}))])=?,
  
    ~disabled: option(bool)=?,
    ~disableFocusRipple: option(bool)=?,
    ~disableRipple: option(bool)=?,
    ~focusVisibleClassName: option(string)=?,
    ~fullWidth: option(bool)=?,
    ~href: option(string)=?,
    ~mini: option(bool)=?,
    ~size: option(size)=?,
    ~type_: option(string)=?,
    ~variant: option(variant)=?,
    ~action: option(Js.t({..}) => unit)=?,
    ~buttonRef: option([
      | `Callback('genericCallback) 
      | `ObjectGeneric(Js.t({..}))])=?,

    ~centerRipple: option(bool)=?,
    ~disableTouchRipple: option(bool)=?,
    ~focusRipple: option(bool)=?,
    ~onBlur: option(ReactEvent.Focus.t => unit)=?,
    ~onClick: option(ReactEvent.Mouse.t => unit)=?,
    ~onFocus: option(ReactEvent.Focus.t => unit)=?,
    ~onFocusVisible: option('genericCallback)=?,
    ~onKeyDown: option(ReactEvent.Keyboard.t => unit)=?,
    ~onKeyUp: option(ReactEvent.Keyboard.t => unit)=?,
    ~onMouseDown: option(ReactEvent.Mouse.t => unit)=?,
    ~onMouseLeave: option(ReactEvent.Mouse.t => unit)=?,
    ~onMouseUp: option(ReactEvent.Mouse.t => unit)=?,
    ~onTouchEnd: option(ReactEvent.Touch.t => unit)=?,
    ~onTouchMove: option(ReactEvent.Touch.t => unit)=?,
    ~onTouchStart: option(ReactEvent.Touch.t => unit)=?,
    ~role: option(string)=?,
    ~tabIndex: option([ 
      | `Int(int) 
      | `Float(float) 
      | `String(string)])=?,

    ~_TouchRippleProps: option(Js.t({..}))=?,
    ~classes: option(Classes.t)=?,
    ~style: option(ReactDOMRe.Style.t)=?,
    children
  ) => {
    ...ReasonReact.statelessComponent("Button.Blended"),
    render: _self => 
      <MaterialUi.Button 
        className=([Style.buttonStyle, className |? ""] >|< " ")
        variant=?(Style.raised ? Some(`Contained) : variant)
        ?color ?component ?disabled ?disableFocusRipple ?disableRipple ?focusVisibleClassName ?fullWidth ?href ?mini ?size ?type_ ?action ?buttonRef ?centerRipple ?disableTouchRipple ?focusRipple ?onBlur ?onClick ?onFocus ?onFocusVisible ?onKeyDown ?onKeyUp ?onMouseUp ?onMouseDown ?onMouseLeave ?onTouchEnd ?onTouchMove ?onTouchStart ?role ?tabIndex ?_TouchRippleProps ?classes ?style
      >
        {children}
      </MaterialUi.Button>
  }
};

module BlendedBase_ {
  let buttonStyle = Css.(
    style([
      color(white),
      fontFamily(Fonts.gobold), 
      fontWeight(`num(300)),
      letterSpacing(pxFloat(0.64))
    ]|> map(rule => !# rule))
  );

  let raised = false;
}

module RaisedBase_ {
  let buttonStyle = Css.(
    style([
      fontFamily(Fonts.jost), 
      fontWeight(`num(500)),
      backgroundColor(white)
    ])
  )

  let raised = true;
}

module Blended = Build(BlendedBase_);
module Raised = Build(RaisedBase_);