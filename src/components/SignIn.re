open Operators;
open Css;

module Styles {
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
    position(absolute),
    top(px(30)),

    Media.atLeast(Media.Breakpoint.Tablet, [
      left(px(24))
    ])
  ])

  let card = style([
    display(`flex),
    flexDirection(`column),
    alignItems(`center),
    justifyContent(`center),

    backgroundColor(hsla(133, 51, 27, 1.0)) |> important,
    Media.atMost(Media.Breakpoint.Tablet, [
      /* TODO: fix important fo box shadow on bs-css */
      /* boxShadow(`transparent) |> important, */
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
    fontSize(px(32))
  ]);

  let textField = style([
    marginTop(`px(8)) |> important,
    width(`percent(100.0))
  ]);

  let button = style([
    marginTop(px(24)),
    marginBottom(px(12)),
    padding2(~v=px(20), ~h=px(0)),
    border(px(1), `solid, hex("ffffff30")),
    width(`percent(80.0))
  ] |> List.map(e => important(e)));

  let actionPanel = style([
    width(`percent(100.0)),
    display(`flex),
    justifyContent(`spaceBetween),
    alignItems(`center)
  ]);

  let checkbox = style([
    color(white) |> important
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
  ])
};

let make = (~title, _children) => {
  ...ReasonReact.statelessComponent("SignIn"),
  render: _self => 
    <div className=Styles.root>
      <Logo.WithCaption className=Styles.logo hideCaptionOnSmall=false caption=title/>
      <MaterialUi.Card className=Styles.card>
        <form className=Styles.form>
          <span className=Styles.welcomeTitle>{ReasonReact.string("Glad to see you back!")}</span>
          <TextField 
            label=ReasonReact.string("email")
            className=Styles.textField/>
          <TextField 
            label=ReasonReact.string("password")
            className=Styles.textField/>
          <div className=Styles.actionPanel>
            <MaterialUi.FormControl>
              <MaterialUi.FormControlLabel 
                label=<span className=Styles.label>{ReasonReact.string("Stay signed in")}</span>
                control=<MaterialUi.Checkbox classes=[
                  MaterialUi.Checkbox.Classes.Root(Styles.checkbox),
                  MaterialUi.Checkbox.Classes.Checked(Styles.checkbox)
                ]/>
              />
            </MaterialUi.FormControl>
            <span className=([Styles.accesoryLabel, Styles.actionLabel] >|< " ")>{ReasonReact.string("Forgot password?")}</span>
          </div>
          <Button.Blended className=Styles.button>"Sign In"</Button.Blended>
          <div className=Styles.signUpContainer>
            <span className=Styles.accesoryLabel>{ReasonReact.string("Don't have an account?")}</span>
            <span className=([Styles.label, Styles.actionLabel] >|< " ")>{ReasonReact.string("Sign up")}</span>
          </div>
        </form>
      </MaterialUi.Card>
    </div>
}