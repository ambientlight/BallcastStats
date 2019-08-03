open PIXI;

type containerNames = {
  playerMarkerPrefix: string,
  tacticRunArrow: string,
};

type generators = {
  playerMarker: (~x: float, ~y: float, ~name: string, ~number: int, ~position: Formation.position, ~colors: Team.colors) => Container.t,
  defensiveRunArrow: (~x: float, ~y: float, ~tx: float, ~ty: float) => Container.t,
  offensiveRunArrow: (~x: float, ~y: float, ~tx: float, ~ty: float) => Container.t,
  pitchTexturePath: string
};

module type T = {
  let playerMarker: (~x: float, ~y: float, ~name: string, ~number: int, ~position: Formation.position, ~colors: Team.colors) => Container.t;
  let defensiveRunArrow: (~x: float, ~y: float, ~tx: float, ~ty: float) => Container.t;
  let offensiveRunArrow: (~x: float, ~y: float, ~tx: float, ~ty: float) => Container.t;
  let pitchTexturePath: string;

  let names: containerNames;

  /***
   * went ahead with functors and realized I am not really able
   * to wrap my head around making skins customizable / dynamic and transitionable
   * for now, going with a simple object bundling these generators
   */
  let bundle: generators;
};

module BaseSkin: T {
  let names = {
    playerMarkerPrefix: "marker",
    tacticRunArrow: "tacticRun"
  };

  [@bs.module] external formationMarker: string = "assets/sprites/formation_marker_default.png";
  [@bs.module] external arrowDefensiveRun: string = "assets/sprites/arrow_defensive_run_v2.png";
  [@bs.module] external arrowOffensiveRun: string = "assets/sprites/arrow_offensive_run_v2.png";
  [@bs.module] external pitchTexturePath: string = "assets/sprites/pitch_overscroll.png";

  let playerMarker = (~x: float, ~y: float, ~name: string, ~number: int, ~position: Formation.position, ~colors: Team.colors): Js.t(#Container._t) => {
    let marker = Sprite.init(
      ~texture=Texture.from(~source=formationMarker),
      ~x, ~y,
      ~width=40.0, ~height=48.0,
      ());

    marker##anchor##set(0.5, 0.5);

    let squadNumber = Text.create(
      ~text=string_of_int(number), 
      ~style=TextStyle.create(
        ~style=TextStyle.style(
          ~fontFamily=[|"Gobold"|],
          ~fontSize=34.0, 
          ~fill=int_of_string("0xffffff"), ())), ());

    squadNumber##anchor##set(0.5, 0.5);
    squadNumber##y #= (-4.0);
    marker##addChild(squadNumber) |> ignore;

    let positionAndName = Text.create(
      ~text=(position |. Formation.positionToJs) ++ " | " ++ name |. Js.String.toUpperCase, 
      ~style=TextStyle.create(
        ~style=TextStyle.style(
          ~fontFamily=[|"Gobold"|], 
          ~fontSize=25.0,
          ~letterSpacing=1.,
          ~fill=int_of_string("0xffffff"), ())), ());

    /* positionAndName##alpha #= 0.85; */
    positionAndName##anchor##set(0.5, 0.5);
    positionAndName##y #= 72.0;
    marker##addChild(positionAndName) |> ignore;

    marker##interactive #= true;
    (marker :> Container.t)
  };

  let _arrowTacticRun = (~texture: Texture.t, ~x: float, ~y: float, ~tx: float, ~ty: float): Js.t(#Container._t) => {
    let anchorsPerBase = 10;
    let anchorsPerArrow = 10;
    let arrowWidthFraction = 0.50;
    
    /* nothing */
    let (points, rotation) = 
    if(x == tx && y == ty){
      ([||], 0.0)
    /* vertical */
    } else if(x == tx){
      let arrowHeight = texture##width *. arrowWidthFraction;
      /* don't get smaller then the arrow size */
      let total = max(abs_float(ty -. y), arrowHeight);
      let baseHeight = total -. arrowHeight; 

      let multiple = ty -. y >= 0.0 ? 1.0 : -1.0;
      let basePoints = Belt.Array.range(1, anchorsPerBase)
        |> Array.map(idx => PIXI.Point.create(~x=0.0, ~y=(float_of_int(idx) /. float_of_int(anchorsPerBase)) *. baseHeight *. multiple, ()) );
      let arrowPoints = Belt.Array.range(1, anchorsPerArrow)
        |> Array.map(idx => PIXI.Point.create(~x=0.0, ~y=(baseHeight +. (float_of_int(idx) /. float_of_int(anchorsPerArrow)) *. arrowHeight) *. multiple, ()) );
      (Array.append(basePoints, arrowPoints), 0.0)
    /* horizontal */
    } else if(y == ty){
      let arrowWidth = texture##width *. arrowWidthFraction;
      /* don't get smaller then the arrow size */
      let total = max(abs_float(tx -. x), arrowWidth);
      let baseWidth = total -. arrowWidth;

      let multiple = tx -. x >= 0.0 ? 1.0 : -1.0;
      let basePoints = Belt.Array.range(1, anchorsPerBase)
        |> Array.map(idx => PIXI.Point.create(~x=(float_of_int(idx) /. float_of_int(anchorsPerBase)) *. baseWidth *. multiple, ~y=0.0, ()) );
      let arrowPoints = Belt.Array.range(1, anchorsPerArrow)
        |> Array.map(idx => PIXI.Point.create(~x=(baseWidth +. (float_of_int(idx) /. float_of_int(anchorsPerArrow)) *. arrowWidth) *. multiple, ~y=0.0, ()) );
      (Array.append(basePoints, arrowPoints), 0.0)
    /* calc the diagonal, and rotate based on the angle with horizontal */
    } else {
      let arrowWidth = texture##width *. arrowWidthFraction;
      let total = max(sqrt((tx -. x) *. (tx -. x) +. (ty -. y) *. (ty -. y)), arrowWidth);
      let baseWidth = total -. arrowWidth;

      let multiple = tx -. x >= 0.0 ? 1.0 : -1.0;
      let basePoints = Belt.Array.range(1, anchorsPerBase)
        |> Array.map(idx => PIXI.Point.create(~x=(float_of_int(idx) /. float_of_int(anchorsPerBase)) *. baseWidth *. multiple, ~y=0.0, ()) );
      let arrowPoints = Belt.Array.range(1, anchorsPerArrow)
        |> Array.map(idx => PIXI.Point.create(~x=(baseWidth +. (float_of_int(idx) /. float_of_int(anchorsPerArrow)) *. arrowWidth) *. multiple, ~y=0.0, ()) );
      
      (Array.append(basePoints, arrowPoints), atan( (ty -. y) /. (tx -. x) ))
    };

    if(points |. Array.length == 0){
      let dummyContainer = PIXI.Container.create(())
      dummyContainer##name #= names.tacticRunArrow;
      dummyContainer
    } else {
      let arrowDefensiveRun: PIXI.SimpleRope.t = PIXI.SimpleRope.create(~texture, ~points);
      arrowDefensiveRun##x #= x;
      arrowDefensiveRun##y #= y;
      arrowDefensiveRun##name #= names.tacticRunArrow;
      arrowDefensiveRun##alpha #= 0.0;
      arrowDefensiveRun##rotation #= rotation;
      (arrowDefensiveRun :> PIXI.Container.t)
    };
  };

  let defensiveRunArrow = (~x: float, ~y: float, ~tx: float, ~ty: float): Js.t(#Container._t) => {
    let texture = Texture.from(~source=arrowDefensiveRun);
    texture##orig #= Rectangle.create(~width=(55.0 /. 2.0), ~height=(33.0 /. 2.0), ());
    
    _arrowTacticRun(~texture, ~x, ~y, ~tx, ~ty);
  }

  let offensiveRunArrow = (~x: float, ~y: float, ~tx: float, ~ty: float): Js.t(#Container._t) => {
    let texture = Texture.from(~source=arrowOffensiveRun);
    texture##orig #= Rectangle.create(~width=(55.0 /. 2.0), ~height=(33.0 /. 2.0), ());

    _arrowTacticRun(~texture, ~x, ~y, ~tx, ~ty);
  }

  let bundle = {
    playerMarker,
    defensiveRunArrow,
    offensiveRunArrow,
    pitchTexturePath
  };
};

module CompactPresentationSkin {
  let names = BaseSkin.names;
  let defensiveRunArrow = (~x: float, ~y: float, ~tx: float, ~ty: float): Js.t(#Container._t) => Container.create();
  let offensiveRunArrow = (~x: float, ~y: float, ~tx: float, ~ty: float): Js.t(#Container._t) => Container.create();
  let pitchTexturePath = BaseSkin.pitchTexturePath;
  //[@bs.module] external pitchTexturePath: string = "assets/sprites/pitch_overscroll_uniform.png";

  let scaleLock = true;

  let playerMarker = (~x: float, ~y: float, ~name: string, ~number: int, ~position: Formation.position, ~colors: Team.colors): Js.t(#Container._t) => {
    let container = Container.create();
    container##y #= y;
    container##x #= x;

    let outer = Graphics.create(());
    outer
    |. Graphics.lineStyle(~width=0.0, ())
    |. Graphics.beginFill(~color=colors.text, ~alpha=1.0)
    |. Graphics.drawCircle(~x=0.0, ~y=0.0, ~radius=20.0)
    |. Graphics.endFill |> ignore;
    container##addChild(outer) |> ignore;

    let outline = Graphics.create(());
    outline
    |. Graphics.lineStyle(~width=0.0, ())
    |. Graphics.beginFill(~color=colors.secondary, ~alpha=1.0)
    |. Graphics.drawCircle(~x=0.0, ~y=0.0, ~radius=16.0)
    |. Graphics.endFill |> ignore;
    container##addChild(outline) |> ignore;

    let inner = Graphics.create(());
    inner
    |. Graphics.lineStyle(~width=0.0, ())
    |. Graphics.beginFill(~color=colors.main, ~alpha=1.0)
    |. Graphics.drawCircle(~x=0.0, ~y=0.0, ~radius=12.0)
    |. Graphics.endFill |> ignore;
    container##addChild(inner) |> ignore;
    
    let squadNumber = Text.create(
      ~text=string_of_int(number), 
      ~style=TextStyle.create(
        ~style=TextStyle.style(
          ~fontFamily=[|"Jost"|],
          ~fontWeight="800",
          ~fontSize=(string_of_int(number) |. String.length) == 1 ? 24.0 : 22.0, 
          ~fill=int_of_string("0xffffff"), ())), ());
    squadNumber##resolution #= 2.0;
    squadNumber##anchor##set(0.5, 0.5);
    squadNumber##x #= 0.0;
    squadNumber##y #= 0.0;
    container##addChild(squadNumber) |> ignore;

    container##interactive #= false;

    container##width #= (container##width /. 1.5);
    container##height #= (container##height /. 1.5);
    container
  };

  let bundle = {
    playerMarker,
    defensiveRunArrow,
    offensiveRunArrow,
    pitchTexturePath
  };
};