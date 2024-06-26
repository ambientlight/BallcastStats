open Css;

module Jost {
  let hairline = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-100-Hairline.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(100),
    ()
  );

  let hairlineItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-100-HairlineItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(100),
    ()
  );

  let thin = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-200-Thin.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(200),
    ()
  );

  let thinItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-200-ThinItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(200),
    ()
  );

  let light = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-300-Light.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(300),
    ()
  );

  let lightItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-300-LightItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(300),
    ()
  );

  let book = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-400-Book.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(400),
    ()
  );

  let bookItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-400-BookItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(400),
    ()
  );

  let medium = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-500-Medium.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(500),
    ()
  );

  let mediumItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-500-MediumItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(500),
    ()
  );

  let semi = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-600-Semi.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(600),
    ()
  );

  let semiItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-600-SemiItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(600),
    ()
  );

  let bold = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-700-Bold.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(700),
    ()
  );

  let boldItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-700-BoldItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(700),
    ()
  );

  let heavy = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-800-Heavy.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(800),
    ()
  );

  let heavyItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-800-HeavyItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(800),
    ()
  );

  let black = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-900-Black.ttf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(900),
    ()
  );

  let blackItalic = fontFace(
    ~fontFamily="Jost",
    ~src=[`url([%bs.raw "require('assets/fonts/Jost/Jost-900-BlackItalic.ttf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(900),
    ()
  );
};

module Gobold {
  let light = fontFace(
    ~fontFamily="Gobold",
    ~src=[`url([%bs.raw "require('assets/fonts/Gobold/Gobold_Light.otf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(200),
    ()
  );

  let lightItalic = fontFace(
    ~fontFamily="Gobold",
    ~src=[`url([%bs.raw "require('assets/fonts/Gobold/Gobold_Light_Italic.otf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(200),
    ()
  );

  let thin = fontFace(
    ~fontFamily="Gobold",
    ~src=[`url([%bs.raw "require('assets/fonts/Gobold/Gobold_Thin.otf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(300),
    ()
  );

  let thinItalic = fontFace(
    ~fontFamily="Gobold",
    ~src=[`url([%bs.raw "require('assets/fonts/Gobold/Gobold_Thin_Italic.otf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(300),
    ()
  );

  let regular = fontFace(
    ~fontFamily="Gobold",
    ~src=[`url([%bs.raw "require('assets/fonts/Gobold/Gobold_Regular.otf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(500),
    ()
  );

  let regularItalic = fontFace(
    ~fontFamily="Gobold",
    ~src=[`url([%bs.raw "require('assets/fonts/Gobold/Gobold_Regular_Italic.otf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(500),
    ()
  );

  let bold = fontFace(
    ~fontFamily="Gobold",
    ~src=[`url([%bs.raw "require('assets/fonts/Gobold/Gobold_Bold.otf')"])],
    ~fontStyle=normal,
    ~fontWeight=`num(800),
    ()
  );

  let boldItalic = fontFace(
    ~fontFamily="Gobold",
    ~src=[`url([%bs.raw "require('assets/fonts/Gobold/Gobold_Bold_Italic.otf')"])],
    ~fontStyle=italic,
    ~fontWeight=`num(800),
    ()
  );
};  

let jost = Jost.medium ++ ", Roboto";
let gobold = Gobold.regular ++ ", Roboto";