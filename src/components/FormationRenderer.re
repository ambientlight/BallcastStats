open Operators;
open Webapi;

module GridManager {
   

};

type textures = {
  pitch: PIXI.Texture.t,
  marker: PIXI.Texture.t
}

type t = {
  textures: textures
};

type assets = {
  pitchTexture: string,
  formationMarker: string
}

let create = (element: Dom.HtmlElement.t, width: int, height: int, assets: assets) => {
  let application = PIXI.Application.create(~options=PIXI.Application.Options.t(
    ~transparent=true,
    ~width=width,
    ~height=height,
    ~resolution=2.0,
    ()), 
  ());

  let style = application |. PIXI.Application.view |. Dom.HtmlElement.style;
  Dom.CssStyleDeclaration.setProperty("width", string_of_int(width) ++ "px", "", style);
  Dom.CssStyleDeclaration.setProperty("height", string_of_int(height) ++ "px", "", style);
  element |> Dom.HtmlElement.appendChild(application |. PIXI.Application.view);

  /* base container */
  let container = PIXI.Container.create();
  application |. PIXI.Application.stage |. PIXI.Container.addChild(container);
  
  let pitchTexture = PIXI.Texture.from(~source=assets.pitchTexture);
  let markerTexture = PIXI.Texture.from(~source=assets.formationMarker);

  let pitch = PIXI.Sprite.create(pitchTexture);
  pitch |. PIXI.Sprite.setWidth(520.0);
  pitch |. PIXI.Sprite.setHeight(400.0);
  container |. PIXI.Container.addChild(!!pitch);

  let marker = PIXI.Sprite.create(markerTexture);
  marker |. PIXI.Sprite.anchor |. PIXI.ObservablePoint.setX(0.5);
  marker |. PIXI.Sprite.anchor |. PIXI.ObservablePoint.setY(0.5);
  marker |. PIXI.Sprite.setWidth(35.0);
  marker |. PIXI.Sprite.setHeight(42.0);
  marker |. PIXI.Sprite.setX(260.0);
  marker |. PIXI.Sprite.setY(200.0);
  container |. PIXI.Container.addChild(!!marker);

  {
    textures: {
      pitch: pitchTexture,
      marker: markerTexture
    }
  }
};