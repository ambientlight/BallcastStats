open PIXI;
open Operators;

let playerMarker = (
  ~markerTexture: Texture.t,
  ~x: float, ~y: float,
  ~playerNumber: int,
  ~playerName: string,
  ~playerPosition: Formation.position) => {
  
  let marker = Sprite.init(
    ~texture=markerTexture,
    ~x,
    ~y,
    ~width=40.0,
    ~height=48.0,
    ()
  );

  marker##name #= {j|marker:$playerNumber|j};
  marker##anchor##set(0.5, 0.5);

  let squadNumber = Text.create(
    ~text=string_of_int(playerNumber), 
    ~style=TextStyle.create(
      ~style=TextStyle.style(
        ~fontFamily=[|"Gobold"|],
        ~fontSize=34.0, 
        ~fill=int_of_string("0xffffff"), ())), ());

  squadNumber##anchor##set(0.5, 0.5);
  squadNumber##y #= (-4.0);
  marker##addChild(squadNumber) |> ignore;

  let positionAndName = Text.create(
    ~text=(playerPosition |. Formation.positionToJs) ++ " | " ++ playerName |. Js.String.toUpperCase, 
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

  marker
};

let arrowTacticRun = (~texture: Texture.t, ~x: float, ~y: float, ~tx: float, ~ty: float): Container.t => {
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
    dummyContainer##name #= "tacticRun";
    dummyContainer
  } else {
    let arrowDefensiveRun: PIXI.SimpleRope.t = PIXI.SimpleRope.create(~texture, ~points);
    arrowDefensiveRun##x #= x;
    arrowDefensiveRun##y #= y;
    arrowDefensiveRun##name #= "tacticRun";
    arrowDefensiveRun##alpha #= 0.0;
    arrowDefensiveRun##rotation #= rotation;
    (arrowDefensiveRun :> PIXI.Container.t)
  };
};