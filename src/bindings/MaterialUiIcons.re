open MaterialUi.SvgIcon;

module type IconRef = {
  let reactClass: ReasonReact.reactClass
};

module BuildIcon = (Base: IconRef) => {
  let reactClass = Base.reactClass;
  
  [@bs.deriving jsConverter]
  type color = [
    | [@bs.as "inherit"] `Inherit
    | [@bs.as "primary"] `Primary
    | [@bs.as "secondary"] `Secondary
    | [@bs.as "action"] `Action
    | [@bs.as "error"] `Error
    | [@bs.as "disabled"] `Disabled
  ];

  [@bs.deriving jsConverter]
  type fontSize = [
    | [@bs.as "inherit"] `Inherit
    | [@bs.as "default"] `Default
    | [@bs.as "small"] `Small
    | [@bs.as "large"] `Large
  ];

  module Classes = {
    type classesType =
      | Root(string)
      | ColorPrimary(string)
      | ColorSecondary(string)
      | ColorAction(string)
      | ColorError(string)
      | ColorDisabled(string)
      | FontSizeInherit(string)
      | FontSizeSmall(string)
      | FontSizeLarge(string);
    type t = list(classesType);
    let to_string =
      fun
      | Root(_) => "root"
      | ColorPrimary(_) => "colorPrimary"
      | ColorSecondary(_) => "colorSecondary"
      | ColorAction(_) => "colorAction"
      | ColorError(_) => "colorError"
      | ColorDisabled(_) => "colorDisabled"
      | FontSizeInherit(_) => "fontSizeInherit"
      | FontSizeSmall(_) => "fontSizeSmall"
      | FontSizeLarge(_) => "fontSizeLarge";
    let to_obj = listOfClasses =>
      listOfClasses|.(
        Belt.List.reduce(
          Js.Dict.empty(),
          (obj, classType) => {
            switch (classType) {
            | Root(className)
            | ColorPrimary(className)
            | ColorSecondary(className)
            | ColorAction(className)
            | ColorError(className)
            | ColorDisabled(className)
            | FontSizeInherit(className)
            | FontSizeSmall(className)
            | FontSizeLarge(className) =>
              Js.Dict.set(obj, to_string(classType), className)
            };
            obj;
          },
        )
      );
  };

  [@bs.obj]
  external makeProps:
    (
      ~className: string=?,
      ~color: string=?,
      ~component: 'union_rpwt=?,
      ~fontSize: string=?,
      ~nativeColor: string=?,
      ~shapeRendering: string=?,
      ~titleAccess: string=?,
      ~viewBox: string=?,
      ~classes: Js.Dict.t(string)=?,
      ~style: ReactDOMRe.Style.t=?,
      unit
    ) =>
    _ =
    "";

  let make =
      (
        ~className: option(string)=?,
        ~color: option(color)=?,
        ~component:
          option(
            [
              | `String(string)
              | `Callback('genericCallback)
              | `ObjectGeneric(Js.t({..}))
            ],
          )=?,
        ~fontSize: option(fontSize)=?,
        ~nativeColor: option(string)=?,
        ~shapeRendering: option(string)=?,
        ~titleAccess: option(string)=?,
        ~viewBox: option(string)=?,
        ~classes: option(Classes.t)=?,
        ~style: option(ReactDOMRe.Style.t)=?,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props=
        makeProps(
          ~className?,
          ~color=?color|.(Belt.Option.map(v => colorToJs(v))),
          ~component=?
            component|.(
                        Belt.Option.map(v => MaterialUi_Helpers.unwrapValue(v))
                      ),
          ~fontSize=?fontSize|.(Belt.Option.map(v => fontSizeToJs(v))),
          ~nativeColor?,
          ~shapeRendering?,
          ~titleAccess?,
          ~viewBox?,
          ~classes=?Belt.Option.map(classes, v => Classes.to_obj(v)),
          ~style?,
          (),
        ),
      children,
    );
};

module Edit_ = {
  [@bs.module "@material-ui/icons/Edit"]
  external reactClass: ReasonReact.reactClass = "default";
};

module Warning_ = {
  [@bs.module "@material-ui/icons/Warning"]
  external reactClass: ReasonReact.reactClass = "default";
};

module Close_ = {
  [@bs.module "@material-ui/icons/Close"]
  external reactClass: ReasonReact.reactClass = "default";
};

module Edit = BuildIcon(Edit_);
module Warning = BuildIcon(Warning_);
module Close = BuildIcon(Close_);