open Operators;
open Css;

let root = style([
  display(`flex),
  alignItems(`center),
  justifyContent(`center),
  flexDirection(`column),
  
  position(fixed),
  left(`zero),
  right(`zero),
  top(`zero),
  bottom(`zero),
  
  Media.atMost(Media.Breakpoint.Phone, [
    backgroundColor(hsla(133, 51, 27, 1.0))
  ]),
  Media.atLeast(Media.Breakpoint.Tablet, [
    backgroundImage(url([%bs.raw "require('assets/svgs/background.svg')"])),
    backgroundSize(`cover)
  ])
]);

let logo = style([
  top(px(20)),
  Media.atLeast(Media.Breakpoint.Phone, [
    top(px(30))
  ]),

  ...Media.rulesSplitOn(Media.Breakpoint.Tablet, 
    ~lower=[
      position(fixed),
      width(`percent(100.0))
    ],
    ~upper=[
      position(absolute),
      left(px(24))
    ])
]);

let card = style([
  display(`flex),
  flexDirection(`column),
  alignItems(`center),
  justifyContent(`center),

  !#backgroundColor(hsla(133, 51, 27, 1.0)),
  Media.atLeast(Media.Breakpoint.Tablet, [
    /* TODO: fix important fo box shadow on bs-css */
    /* !#boxShadow(`transparent), */
    unsafe("box-shadow", "0 0 0 0 transparent !important")
  ]),

  ...Media.property2SplitOn(Media.Breakpoint.Tablet, ~rule=(Css.width, Css.height), 
    ~lower=(`percent(100.0), `percent(100.0)), 
    ~upper=(`px(500), `px(400))
  )
]);

let form = style([
  display(`flex),
  flexDirection(`column),
  alignItems(`center),

  width(`percent(100.0)),
  padding2(~v=px(0), ~h=px(32))
]);

let welcomeTitle = style([
  fontFamily(Fonts.jost),
  fontWeight(`num(300)),

  color(white),
  fontSize(px(32)),

  ...Media.propertySplitOn(Media.Breakpoint.Phone, ~rule=display,
    ~lower=`none,
    ~upper=`initial
  )
]);

let textField = style([
  width(`percent(100.0)),
]);

let button = style([
  marginTop(px(24)),
  marginBottom(px(12)),
  border(px(1), `solid, rgba(255, 255, 255, 0.3)),
  width(`percent(80.0)),

  ...Media.rulesSplitOn(Media.Breakpoint.Phone,
    ~lower=[!# padding2(~v=px(12), ~h=px(0))],
    ~upper=[!# padding2(~v=px(20), ~h=px(0))]
  )
] |> List.map(rule => !# rule));

let actionPanel = style([
  width(`percent(100.0)),
  display(`flex),
  justifyContent(`spaceBetween),
  alignItems(`center)
]);

let checkbox = style([
  !# color(white)
]);

let label = style([
  color(white),
  fontFamily(Fonts.jost)
]);

let accesoryLabel = style([
  color(white),
  fontFamily(Fonts.jost),
  opacity(0.50)
]);

let actionLabel = style([
  selector("&:hover", [
    textDecoration(`underline),
    cursor(`pointer)
  ])
]);

let signUpContainer = style([
  selector("&>*", [
    margin2(~v=px(0), ~h=px(4))
  ])
]);