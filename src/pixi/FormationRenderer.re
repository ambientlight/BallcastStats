open Operators;
open Webapi;
open PIXI;

type position = {
  x: float,
  y: float
};

module GridManager {
  let gridCellPosition = (~withSize as size: int, ~x: int, ~y: int): position => {
    x: float(x * size),
    y: float(y * size)
  }
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
  
  let container = Viewport.(
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
  |. EventEmitter.on(~event="zoomed", ~fn=(event: Js.t({.. viewport: Viewport.t})) => { 
    event##viewport##children
    |. Belt.Array.keep(child => child##name |. Js.Nullable.toOption == Some("marker"))
    |. Belt.Array.map(child => { let container: Container.t = !!child; container })
    |. Belt.Array.forEach(child => {
      child##width #= (45.0 /. (event##viewport##lastViewport##scaleX));
      child##height #= (53.0 /. (event##viewport##lastViewport##scaleY));
    }); 
  }, ())
  |. EventEmitter.on(~event="zoomed-end", ~fn=(viewport: Viewport.t) => (), ())
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
    let gridPosition = GridManager.gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y)
    let marker = Sprite.init(
      ~texture=renderer.textures.marker,
      ~x=gridPosition.x,
      ~y=gridPosition.y,
      ~width=45.0,
      ~height=53.0,
      ()
    );

    marker##name #= "marker";
    marker##anchor##set(0.5, 0.5);

    let numberStyle = TextStyle.create(~style=TextStyle.style(~fontFamily=[|"Gobold"|], ~fontSize=32.0, ~fill=int_of_string("0xffffff"), ()));
    let text = Text.create(~text=string_of_int(squad[index].number), ~style=numberStyle, ());
    text##anchor##set(0.5, 0.5);
    text##y #= (-4.0);
    marker##addChild(text) |> ignore;

    let nameStyle = TextStyle.create(TextStyle.style(~fontFamily=[|"Gobold"|], ~fontSize=24.0, ~fill=int_of_string("0xffffff"), ()));
    let text = Text.create(~text=(element.position |. Formation.positionToJs) ++ " | " ++ squad[index].name |. Js.String.toUpperCase, ~style=nameStyle, ());
    text##anchor##set(0.5, 0.5);
    text##y #= 72.0;
    marker##addChild(text) |> ignore;
    
    renderer.container##addChild(marker) |> ignore;
  })
};