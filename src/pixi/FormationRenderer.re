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
  /* let containerC = ContainerC.create(); */
  
  application |. Application.stage |. Container.addChild(container);
  
  let pitchTexture = Texture.from(~source=assets.pitchTexture);
  let markerTexture = Texture.from(~source=assets.formationMarker);

  let pitch = Sprite.create(pitchTexture);
  pitch |. Sprite.setWidth(520.0);
  pitch |. Sprite.setHeight(400.0);
  pitch |. Sprite.setName("pitch");
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

let loadFormation = (renderer: t, formation: Formation.t, squad: Formation.squad) => {
  renderer.container |. Container.children 
  |. Utils.Array.optMap(element => element |. Js.Nullable.toOption)
  |. Belt.Array.forEach(element => {
    let name = (element |. PIXI.Container.name |. Js.Nullable.toOption);
    if(name == Some("marker")){
      renderer.container |. Container.removeChild(element);
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

      marker |. Sprite.setName("marker");
      (marker |. Sprite.anchor)##set(0.5, 0.5);



      let numberStyle = TextStyle.create(TextStyle.style(~fontFamily=[|"Gobold"|], ~fontSize=32.0, ~fill=int_of_string("0xffffff"), ()));
      let text = Text.create(~text=string_of_int(squad[index].number), ~style=numberStyle, ());
      (text |. Text.anchor)##set(0.5, 0.5);
      text |. Text.setY(-4.0);
      marker |. Sprite.addChild(!!text);

      let nameStyle = TextStyle.create(TextStyle.style(~fontFamily=[|"Gobold"|], ~fontSize=24.0, ~fill=int_of_string("0xffffff"), ()));
      let text = Text.create(~text=(element.position |. Formation.positionToJs) ++ " | " ++ squad[index].name |. Js.String.toUpperCase, ~style=nameStyle, ());
      (text |. Text.anchor)##set(0.5, 0.5);
      text |. Text.setY(72.0);
      marker |. Sprite.addChild(!!text);
      
      renderer.container |. Container.addChild(!!marker);

      let equal = IPoint.equal(
        `ObservablePoint(marker |. Sprite.anchor),
        `ObservablePoint(marker |. Sprite.anchor));
      ~~equal;

      let point = Point.create(~x=0.0, ~y=0.0);
      point |. IPoint.copyPointFrom(`ObservablePoint(marker |. Sprite.anchor)) |> ignore;
      ~~point;
      ()
    })
};