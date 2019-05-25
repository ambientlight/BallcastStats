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
}

let create = (element: Dom.HtmlElement.t, width: int, height: int, assets: assets) => {
  let application = Application.create(~options=Application.Options.t(
    ~transparent=true,
    ~width=width,
    ~height=height,
    ~resolution=2.0,
    ()), 
  ());

  let style = application |. Application.view |. Dom.HtmlElement.style;
  Dom.CssStyleDeclaration.setProperty("width", string_of_int(width) ++ "px", "", style);
  Dom.CssStyleDeclaration.setProperty("height", string_of_int(height) ++ "px", "", style);
  element |> Dom.HtmlElement.appendChild(application |. Application.view);

  /* base container */
  let container = Container.create();
  application |. Application.stage |. Container.addChild(container);
  
  let pitchTexture = Texture.from(~source=assets.pitchTexture);
  let markerTexture = Texture.from(~source=assets.formationMarker);

  let pitch = Sprite.create(pitchTexture);
  pitch |. Sprite.setWidth(520.0);
  pitch |. Sprite.setHeight(400.0);
  container |. Container.addChild(!!pitch);

  {
    application,
    container,
    textures: {
      pitch: pitchTexture,
      marker: markerTexture
    }
  }
};

let loadFormation = (renderer: t, formation: Formation.t) => {
  formation.elements
  |. Belt.Array.forEachWithIndex((index, element) => {
      let gridPosition = GridManager.gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y)
      let marker = Sprite.init(
        ~texture=renderer.textures.marker,
        ~x=gridPosition.x,
        ~y=gridPosition.y,
        ~width=35.0,
        ~height=42.0,
        ()
      );
      marker |. Sprite.anchor |. ObservablePoint.setPosition(~x=0.5, ~y=0.5);

      let textStyle = TextStyle.create(TextStyle.style(~fontFamily=[|"Gobold"|], ~fontSize=30.0, ~fill=int_of_string("0xffffff"), ()));
      let text = Text.create(~text=string_of_int(index+1), ~style=textStyle, ());
      text |. Text.anchor |. ObservablePoint.setPosition(~x=0.5, ~y=0.5);
      marker |. Sprite.addChild(!!text);

      renderer.container |. Container.addChild(!!marker);
    })
};