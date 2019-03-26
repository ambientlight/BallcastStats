open Operators;
open Css;

let root = MaterialUi.Theme.(style([
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
    backgroundColor(
      raw(AppTheme.theme|.Theme.paletteGet|.Palette.primaryGet|.PaletteColor.mainGet)
    )
  ]),
  Media.atLeast(Media.Breakpoint.Tablet, [
    backgroundImage(url([%bs.raw "require('assets/svgs/background.svg')"])),
    backgroundSize(`cover)
  ])
]));

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

/* in sign up, to fit all 3 inputs cleanly, hide the logo */
let hideLogoHackOnNarrowLayout = style([
  display(`none),
  media("(min-height: 430px)", [
    display(`flex)
  ])
]);

let card = MaterialUi.Theme.(style([
  display(`flex),
  flexDirection(`column),
  alignItems(`center),
  justifyContent(`center),

  !#backgroundColor(
    raw(AppTheme.theme|.Theme.paletteGet|.Palette.primaryGet|.PaletteColor.mainGet)
  ),
  Media.atLeast(Media.Breakpoint.Tablet, [
    /* TODO: fix important fo box shadow on bs-css */
    /* !#boxShadow(`transparent), */
    unsafe("box-shadow", "0 0 0 0 transparent !important")
  ]),

  ...Media.property2SplitOn(Media.Breakpoint.Tablet, ~rule=(Css.width, Css.height), 
    ~lower=(`percent(100.0), `percent(100.0)), 
    ~upper=(`px(500), `px(450))
  )
]));

let form = style([
  display(`flex),
  flexDirection(`column),
  alignItems(`center),

  width(`percent(100.0)),
  padding2(~v=px(0), ~h=px(32)),

  selector("
    input:-webkit-autofill,
    input:-webkit-autofill:hover, 
    input:-webkit-autofill:focus, 
    input:-webkit-autofill:active
  ", [
    unsafe("-webkit-box-shadow", "0 0 0 30px " ++ MaterialUi.Theme.(AppTheme.theme|.Theme.paletteGet|.Palette.primaryGet|.PaletteColor.mainGet) ++ " inset !important"),
    unsafe("-webkit-text-fill-color", MaterialUi.Theme.(AppTheme.theme|.Theme.paletteGet|.Palette.primaryGet|.PaletteColor.contrastTextGet) ++ " !important")
  ])
]);

let welcomeTitle = style([
  Media.below(Media.Breakpoint.Phone, [!#display(`none)]),
  Media.atLeast(Media.Breakpoint.Phone, [!#display(`initial)])
]);

let textField = style([
  width(`percent(100.0)),
  /* default margin is a bit too small so the label easily hits above objects */
  !# marginTop(`px(8))
]);

let button = style([
  marginTop(px(24)),
  marginBottom(px(12)),
  width(`percent(80.0)),
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

let accesoryLabel = style([
  color(white),
  fontFamily(Fonts.jost),
  opacity(0.50)
]);

let additionalInfoLabel = style([
  color(white),
  fontFamily(Fonts.jost),
  fontSize(px(12)),
  opacity(0.50)
]);

let actionLabel = style([
  selector("&:hover", [
    textDecoration(`underline),
    cursor(`pointer)
  ]),
  opacity(1.0)
]);

let signUpContainer = style([
  selector("&>*", [
    margin2(~v=px(0), ~h=px(4))
  ])
]);

let additionalInfoContainer = style([
  textAlign(`center),
  marginTop(px(8)),
  selector("&>*", [
    margin2(~v=px(0), ~h=px(2))
  ])
]);

let smallTopMargin = style([
  marginTop(px(32))
]);

let progressSpinner = style([
  !# color(white)
]);

/* input[type="number"]::-webkit-outer-spin-button,
  input[type="number"]::-webkit-inner-spin-button {
      -webkit-appearance: none;
      margin: 0;
  }
  input[type="number"] {
      -moz-appearance: textfield;
  } */


let codeInputBase = style([
  marginTop(px(8)),

  selector("input[type=\"number\"]::-webkit-outer-spin-button, input[type=\"number\"]::-webkit-inner-spin-button", [
    unsafe("-webkit-appearance", "none"),
    margin(zero)
  ]),

  selector("input[type=\"number\"]", [
    unsafe("-moz-appearance", "textfield")
  ]),

  selector("&>input", [
    fontFamily(Fonts.gobold),
    borderRadius(px(4)),
    margin(px(4)),
    paddingLeft(px(9)),
    width(px(36)),
    height(px(42)),
    fontSize(px(32)),
    boxSizing(`borderBox),
    
    selector("::selection", [
      backgroundColor(transparent)
    ])
  ])
])

let normalCodeInput = style([
  /* make sure these are not override by ReactCodeInput disabled element inline style */
  selector("&>input", [
    !#border(px(1), `solid, hsl(133, 39, 45)),
    !#color(transparent),
    !#backgroundColor(`hex("308141")),
    textShadow(white),
    outlineColor(hsl(133, 39, 60))
  ])
])

let errorCodeInput = style([
  selector("&>input", [
    !#border(px(1), `solid, `hex("dc4909")),
    !#color(transparent),
    !#backgroundColor(`hex("8c6322")),
    textShadow(`hex("dc4909")),
    outlineColor(`hex("f7733b"))
  ])
])

let disabledCodeInput = style([
  opacity(0.5),
  selector("&>input", [
    outlineStyle(`none)
  ])
])