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
    ~resolution=2.0,
    ()), 
  ());

  let style = application##view |. Dom.HtmlElement.style;
  Dom.CssStyleDeclaration.setProperty("width", string_of_int(width) ++ "px", "", style);
  Dom.CssStyleDeclaration.setProperty("height", string_of_int(height) ++ "px", "", style);
  element |> Dom.HtmlElement.appendChild(application##view);

  /* base container */
  let container = Container.create();
  application##stage##addChild(container) |> ignore;
  let pitchTexture = Texture.from(~source=assets.pitchTexture);
  let markerTexture = Texture.from(~source=assets.formationMarker);

  let pitch = Sprite.create(pitchTexture);
  pitch##width #= 520.0;
  pitch##height #= 400.0;
  pitch##name #= "pitch";
  container##addChild(pitch) |> ignore;

  {
    application,
    container,
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