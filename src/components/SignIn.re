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
  ])

  let inputLabelProps = [%bs.obj {
    classes: {
      root: style([
        fontFamily(Fonts.jost),
        textTransform(`uppercase),
        color(white)
      ] |> List.map(e => important(e)))
    }
  }];

  let inputProps = [%bs.obj {
    classes: {
      root: style([
        color(white) |> important
      ]),
      underline: style([
        borderBottom(px(1), `solid, white) |> important,
        selector("&:after", [
          borderBottom(px(1), `solid, white) |> important
        ])
      ])
    }
  }];
};

let make = (~title, _children) => {
  ...ReasonReact.statelessComponent("SignIn"),
  render: _self => 
    <div className=Styles.root>
      <Logo.WithCaption className=Styles.logo hideCaptionOnSmall=false caption=title/>
      <MaterialUi.Card className=Styles.card>
        <form className=Styles.form>
          <span className=Styles.welcomeTitle>{ReasonReact.string("Glad to see you back!")}</span>
          <MaterialUi.TextField 
            label=ReasonReact.string("email")
            className=Styles.textField
            _InputLabelProps=Styles.inputLabelProps 
            _InputProps=Styles.inputProps/>
          <MaterialUi.TextField 
            label=ReasonReact.string("password")
            className=Styles.textField
            _InputLabelProps=Styles.inputLabelProps 
            _InputProps=Styles.inputProps/>
          
        </form>
      </MaterialUi.Card>
    </div>
}