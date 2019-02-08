open Operators;
open Css;

module TopCTA {
  let root = style([
    display(`flex),
    flexDirection(`column),
    alignItems(`center),
    
    padding2(~v=px(24), ~h=px(36)),
    Media.atLeast(Media.Breakpoint.Tablet, [
      padding2(~v=px(50), ~h=px(84))
    ])
  ]);

  let title = style([
    marginBottom(px(36)),

    fontFamily(Fonts.jost),
    color(white),

    ...Media.property2SplitOn(Media.Breakpoint.Phone, ~rule=(fontSize, lineHeight), 
      ~lower=(px(28), px(36)),
      ~upper=(px(42), px(58))
    )
  ]);

  let subtitle = style([
    marginBottom(px(54)),

    fontFamily(Fonts.jost),
    fontSize(px(16)),
    Media.atLeast(Media.Breakpoint.Tablet, [
      fontSize(px(20))
    ]),

    opacity(0.73),
    color(white),

    ...Media.propertySplitOn(Media.Breakpoint.Phone, ~rule=display,
      ~lower=`none,
      ~upper=`initial
    )
  ])

  let ctaButton = style([
    color(hex("277939")),
    width(px(240)),
    height(px(80)),
    fontSize(px(18)),
    borderRadius(px(2)),
    fontWeight(`num(500)),
  ] |> List.map(rule => !# rule))
};

module TopPreview {
  let root = style([
    margin(px(36)),

    backgroundImage(url([%bs.raw "require('assets/images/dummy_preview@2x.png')"])),
    backgroundRepeat(`noRepeat),
    backgroundSize(`contain),
    backgroundPosition(`percent(50.0), `percent(50.0)),

    /*
     * parent layout(see Root below) doesn't define height for below laptop layouts 
     * so explicit height is required
     */
    Media.atMost(Media.Breakpoint.Tablet, [
      height(`px(524))
    ]),

    /* hack for safari so top preview occupies enough space */
    Media.above(Media.Breakpoint.Tablet, [
      minHeight(`vh(75.0))
    ])
  ]);
};

module Root {
  let root = style([
    display(grid),
    /* on safari second row won't stretch to the parent height unfortunately */
    gridTemplateRows([`auto, `minMax(`zero, `fr(1.0))]),
    gridTemplateColumns([`repeat(`num(12), `fr(1.0))]),
    Media.atMost(Media.Breakpoint.Tablet, [
      gridTemplateColumns([`repeat(`num(5), `fr(1.0))]),
    ]),
  
    /*
     * since we cannot use vh safely, this allows second grid row to occupy remaining screen space 
     * unfortunately this doesn't really work with safari
     */
    Media.above(Media.Breakpoint.Tablet, [
      position(fixed),
      left(`zero),
      right(`zero),
      top(`zero),
      bottom(`zero)
    ]),
  
    backgroundImage(url([%bs.raw "require('assets/svgs/background.svg')"])),
    backgroundSize(`cover)
  ]);

  /** grid item placements */
  let navHeader = style([gridColumn(`num(1), `num(-1))]);
  let topCTA = style([gridColumnStart(`spanNum(5))]);
  let topPreview = style([gridColumnStart(`spanNum(7))]);
}