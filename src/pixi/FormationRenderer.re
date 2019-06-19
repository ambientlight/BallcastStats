open Operators;
open Webapi;
open PIXI;

[@bs.module] external arrowDefensiveRun: string = "assets/sprites/arrow_defensive_run.png";
[@bs.module] external arrowOffensiveRun: string = "assets/sprites/arrow_offensive_run.png";

type position = {
  x: float,
  y: float
};

let gridCellPosition = (~withSize as size: int, ~x: int, ~y: int): position => {
  x: float(x * size),
  y: float(y * size)
};

type textures = {
  pitch: Texture.t,
  marker: Texture.t
}

type t = {
  application: Application.t,
  container: Container.t,
  textures: textures
};

type assets = {
  pitchTexture: string,
  formationMarker: string
};

let create = (element: Dom.HtmlElement.t, width: int, height: int, assets: assets) => {
  let application = Application.create(~options=Application.options(
    ~transparent=true,
    ~width=width,
    ~height=height,
    ~resolution=(!!DomRe.window) |. Js.Dict.unsafeGet("devicePixelRatio"),
    ()), 
  ());

  let style = application##view |. Dom.HtmlElement.style;
  Dom.CssStyleDeclaration.setProperty("width", string_of_int(width) ++ "px", "", style);
  Dom.CssStyleDeclaration.setProperty("height", string_of_int(height) ++ "px", "", style);
  element |> Dom.HtmlElement.appendChild(application##view);

  /* base container */
  let containerWidth = float(width);
  let containerHeight = float(height);
  let applicationInterraction: (. Application.t) => InteractionManager.t = [%raw {|function(application){ return application.renderer.plugins.interaction }|}];
  
  let container = PIXIViewport.(
    create(~options=createOptions(
      ~screenWidth=containerWidth *. 1.0,
      ~screenHeight=containerHeight *. 1.0,
      ~worldWidth=containerWidth *. 1.0,
      ~worldHeight=containerHeight *. 1.0,
      ~passiveWheel=true, 
      ~interaction=applicationInterraction(. application), ()),
      ())
    |. drag(~options=`ClampWheelBool(dragOptionsClampWheelBool(~clampWheel=true, ())), ())
    |. wheel(())
    |. decelerate(())
    |. pinch(~options=pinchOptions(~percent=2.0, ()), ())
    |. bounce(~options=`EaseString(bounceOptionsEaseString(
      ~sides="all", 
      ~underflow="center", 
      ~time=300,
      ~ease="easeInOutSine", ())), 
      ())
    |. clampZoom(~options=clampZoomOptions(
      ~minWidth=containerWidth /. 2.0,
      ~minHeight=containerHeight /. 2.0,
      ~maxWidth=containerWidth,
      ~maxHeight=containerHeight, ()),
      ())
  );

  container 
  |. EventEmitter.on(~event="zoomed", ~fn=(event: Js.t({.. viewport: PIXIViewport.t})) =>
    event##viewport##children
    |. Belt.Array.keep(child => child##name |. Js.Nullable.toOption == Some("marker"))
    |. Belt.Array.map(child => { let container: Container.t = !!child; container })
    |. Belt.Array.forEach(child => {
      child##width #= (45.0 /. (event##viewport##lastViewport##scaleX));
      child##height #= (53.0 /. (event##viewport##lastViewport##scaleY));
    }), ())
  |. EventEmitter.on(~event="zoomed-end", ~fn=(viewport: PIXIViewport.t) => (), ())
  |> ignore;


  application##stage##addChild(container) |> ignore;
  let pitchTexture = Texture.from(~source=assets.pitchTexture);
  let markerTexture = Texture.from(~source=assets.formationMarker);

  let pitch = Sprite.create(pitchTexture);
  pitch##width #= 1560.0;
  pitch##height #= 1200.0;
  pitch##anchor##set(0.333, 0.333);
  pitch##name #= "pitch";
  container##addChild(pitch) |> ignore;

  {
    application,
    container: (container :> Container.t),
    textures: {
      pitch: pitchTexture,
      marker: markerTexture
    }
  }
};

let loadFormation = (renderer: t, formation: Formation.t, squad: Formation.squad) => {
  renderer.container##children 
  /* null children appear on hot reload somehow */
  |. Utils.Array.optMap(element => !!element |. Js.Nullable.toOption)
  |. Belt.Array.forEach(element => {
    let name = (element##name |. Js.Nullable.toOption);
    if(name == Some("marker")){
      renderer.container##removeChild(element) |> ignore;
    }
  });

  formation.elements
  |. Belt.Array.forEachWithIndex((index, element) => {
    let gridPosition = gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y);
    let playerMarker = FormationItemsGenerators.playerMarker(
      ~markerTexture=renderer.textures.marker,
      ~x=gridPosition.x, ~y=gridPosition.y,
      ~playerNumber=squad[index].number,
      ~playerName=squad[index].name,
      ~playerPosition=element.position
    );
    
    renderer.container##addChild(playerMarker) |> ignore;

    if(index == 2){
      let arrowTacticsRunTexture = Texture.from(~source=arrowDefensiveRun);
      let rect = Rectangle.create(
        ~width=(55.0 /. 2.0),
        ~height=(33.0 /. 2.0), ());
      arrowTacticsRunTexture##orig #= rect;

      let arrowTacticRun = FormationItemsGenerators.arrowTacticRun(
        ~texture=arrowTacticsRunTexture,
        ~x=gridPosition.x,
        ~y=gridPosition.y);

      renderer.container##addChild(arrowTacticRun) |> ignore;
    };

    ()
  })
};