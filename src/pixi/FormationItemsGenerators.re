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

  marker##name #= "marker";
  marker##anchor##set(0.5, 0.5);

  let squadNumber = Text.create(
    ~text=string_of_int(playerNumber), 
    ~style=TextStyle.create(
      ~style=TextStyle.style(
        ~fontFamily=[|"Gobold Thin"|],
        ~fontSize=34.0, 
        ~fill=int_of_string("0xffffff"), ())), ());

  squadNumber##anchor##set(0.5, 0.5);
  squadNumber##y #= (-4.0);
  marker##addChild(squadNumber) |> ignore;

  let positionAndName = Text.create(
    ~text=(playerPosition |. Formation.positionToJs) ++ " | " ++ playerName |. Js.String.toUpperCase, 
    ~style=TextStyle.create(
      ~style=TextStyle.style(
        ~fontFamily=[|"Gobold Thin"|], 
        ~fontSize=25.0,
        ~letterSpacing=1.,
        ~fill=int_of_string("0xffffff"), ())), ());

  /* positionAndName##alpha #= 0.85; */
  positionAndName##anchor##set(0.5, 0.5);
  positionAndName##y #= 72.0;
  marker##addChild(positionAndName) |> ignore;

  marker
};

let arrowTacticRun = (~texture: Texture.t, ~x:float, ~y:float, ~tx:float, ~ty:float) => {
  /* divide the thing into 20 anchors */
  let anchorCount = 20.0;
  let ropeLength = (texture##width  /. anchorCount);
  /* a head is a half of texture */
  let arrowHeadFractionToWidth = 0.5;
  let arrowHeadWidth = texture##width *. arrowHeadFractionToWidth;

  let arrowWidth = tx -. x;
  let baseWidth = arrowWidth -. arrowHeadWidth;

  let headStartIdx = arrowHeadFractionToWidth *. anchorCount;
  
  let distBetweenBaseAnchor = baseWidth /. (anchorCount *. (1.0 -. arrowHeadFractionToWidth));
  let distBetweenHeadAnchor = arrowHeadWidth /. (anchorCount *. arrowHeadFractionToWidth) *. (distBetweenBaseAnchor > 0.0 ? 1.0 : -1.0);

  ~~({
    "arrowHeadWidth": arrowHeadWidth,
    "arrowWidth": arrowWidth,
    "baseWidth": baseWidth,
    "headStartIdx": headStartIdx,
    "distBetweenBaseAnchor": distBetweenBaseAnchor,
    "distBetweenHeadAnchor": distBetweenHeadAnchor
  });

  let ratio = abs_float((ty -. y) /. (tx -. x));

  let yOffset = 4.0;
  let baseY = (y +. yOffset);
  let points = 
    Belt.Array.range(0, int_of_float(anchorCount) - 1)
    |> Array.map(idx => PIXI.Point.create(
      ~x=float_of_int(idx) < headStartIdx
        ? distBetweenBaseAnchor *. float_of_int(idx)
        : (distBetweenBaseAnchor *. headStartIdx) +. (float_of_int(idx) -. headStartIdx) *. distBetweenHeadAnchor, 
      
      /* CLEAN THIS S UP */
      ~y=float_of_int(idx) < headStartIdx
        ? ((ty -. y) *. ratio *. float_of_int(idx) /. anchorCount) 
        : ((ty -. y) *. ratio *. headStartIdx /. anchorCount) +. ((float_of_int(idx) -. headStartIdx) *. (ty -. y) *. ratio *. (distBetweenHeadAnchor /. distBetweenBaseAnchor) /. anchorCount),
      ()));
    
  let arrowDefensiveRun: PIXI.SimpleRope.t = PIXI.SimpleRope.create(~texture, ~points);
  arrowDefensiveRun##x #= x;
  arrowDefensiveRun##y #= baseY;
  arrowDefensiveRun##name #= "tacticRun";
  arrowDefensiveRun##alpha #= 0.0;
  arrowDefensiveRun
};