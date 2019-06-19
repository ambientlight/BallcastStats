open PIXI;

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
    ~width=45.0,
    ~height=53.0,
    ()
  );

  marker##name #= "marker";
  marker##anchor##set(0.5, 0.5);

  let squadNumber = Text.create(
    ~text=string_of_int(playerNumber), 
    ~style=TextStyle.create(
      ~style=TextStyle.style(~fontFamily=[|"Gobold"|], 
      ~fontSize=32.0, 
      ~fill=int_of_string("0xffffff"), ())), ());

  squadNumber##anchor##set(0.5, 0.5);
  squadNumber##y #= (-4.0);
  marker##addChild(squadNumber) |> ignore;

  let positionAndName = Text.create(
    ~text=(playerPosition |. Formation.positionToJs) ++ " | " ++ playerName |. Js.String.toUpperCase, 
    ~style=TextStyle.create(
      ~style=TextStyle.style(~fontFamily=[|"Gobold"|], 
      ~fontSize=24.0, 
      ~fill=int_of_string("0xffffff"), ())), ());
  positionAndName##anchor##set(0.5, 0.5);
  positionAndName##y #= 72.0;
  marker##addChild(positionAndName) |> ignore;

  marker
};

let arrowTacticRun = (~texture: Texture.t, ~x:float, ~y:float) => {
  let ropeLength = (texture##width /. 20.0);
  let points = 
    Belt.Array.range(0, 19)
    |> Array.map(idx => PIXI.Point.create(~x=float_of_int(idx) *. ropeLength, ~y=0.0, ()));
    
  let arrowDefensiveRun: PIXI.SimpleRope.t = PIXI.SimpleRope.create(~texture, ~points);
  arrowDefensiveRun##x #= x;
  arrowDefensiveRun##y #= (y +. (8.0 /. 4.0));
  arrowDefensiveRun
};