open MaterialUi.SvgIcon;

module type IconRef = {
  let reactClass: ReasonReact.reactClass
};

module BuildIcon = (Base: IconRef) => {
  let reactClass = Base.reactClass;
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
      ~titleAccess: option(string)=?,
      ~viewBox: option(string)=?,
      ~classes: option(Classes.t)=?,
      ~style: option(ReactDOMRe.Style.t)=?,
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=Base.reactClass,
    ~props=
      makeProps(
        ~className?,
        ~color=?color |. Belt.Option.map(v => colorToJs(v)),
        ~component=?
          component |. Belt.Option.map(v => MaterialUi_Helpers.unwrapValue(v)),
        ~fontSize=?fontSize |. Belt.Option.map(v => fontSizeToJs(v)),
        ~nativeColor?,
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