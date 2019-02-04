open Css;

let root = style([
  height(vh(100.0)),
  padding(px(0)),
  backgroundImage(url([%bs.raw "require('assets/svgs/background.svg')"]))
]);

let typography = style([
  important(fontFamily(Fonts.jost))
]);