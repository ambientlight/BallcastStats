open Operators;
open Webapi;
open PIXI;
open PIXIExtras;

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
};

type state = {
  formation: Formation.t,
  squad: Formation.squad
};


type pitchScene = {
  application: Application.t,
  container: Viewport.t,
  state: option(state)
};

module type PitchScene {
  let create: (~element: Dom.HtmlElement.t, ~width: int, ~height: int) => pitchScene;
  let handleZoom: Viewport.t => unit;
  let loadFormation: (pitchScene, ~formation: Formation.t, ~squad: Formation.squad) => pitchScene;
  let transitionTo: (pitchScene, ~formation: Formation.t, ~labels: bool) => Rx.Observable.t(pitchScene);
};

module BuildPitchScene = (Skin: FormationSkin.T): PitchScene => {
  let handleZoom = (viewport: Viewport.t) => {

    if(!Skin.scaleLock){
      viewport##children
      |. Belt.Array.keep(child =>
        child##name 
        |. Js.Nullable.toOption
        |. Belt.Option.getWithDefault("")
        |> Js.String.startsWith(Skin.names.playerMarkerPrefix))
      |. Belt.Array.map(child => { let container: Container.t = !!child; container })
      |. Belt.Array.forEach(child => {
        let lastViewport = (viewport##lastViewport |. Js.Undefined.toOption) |? [%bs.obj {
          scaleX: viewport##scale##x,
          scaleY: viewport##scale##y,
          x: 0.0,
          y: 0.0
        }];
        child##width #= (40.0 /. (lastViewport##scaleX));
        child##height #= (48.0 /. (lastViewport##scaleY));
      });
    }

    viewport##children
    |. Belt.Array.keep(child => child##name |. Js.Nullable.toOption == Some(Skin.names.tacticRunArrow))
    |. Belt.Array.map(child => { let container: Container.t = !!child; container })
    |. Belt.Array.forEach(child => {
      let lastViewport = (viewport##lastViewport |. Js.Undefined.toOption) |? [%bs.obj {
        scaleX: viewport##scale##x,
        scaleY: viewport##scale##y,
        x: 0.0,
        y: 0.0
      }];

      child##alpha #= (lastViewport##scaleX -. 1.0);
    });
  };

  let create = (~element: Dom.HtmlElement.t, ~width: int, ~height: int) => {
    let application = Application.create(~options=Application.options(
      ~transparent=true,
      ~width=width,
      ~height=height,
      ~antialias=true,
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
    
    let container = Viewport.create(~options=Viewport.createOptions(
      ~screenWidth=containerWidth *. 1.0,
      ~screenHeight=containerHeight *. 1.0,
      ~worldWidth=containerWidth *. 1.0,
      ~worldHeight=containerHeight *. 1.0,
      ~passiveWheel=true, 
      ~interaction=applicationInterraction(. application), ()),
      ());
    
    let container = Viewport.(
      container
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
        ()));

    if(!Skin.scaleLock){
      container 
      |. EventEmitter.on(~event="zoomed", ~fn=(event: Js.t({.. viewport: Viewport.t})) => handleZoom(event##viewport), ())
      |. EventEmitter.on(~event="zoomed-end", ~fn=(viewport: Viewport.t) => (), ())
      |> ignore;
    }

    application##stage##addChild(container) |> ignore;
    let pitchTexture = Texture.from(~source=Skin.pitchTexturePath);
    let pitch = Sprite.create(pitchTexture);
    pitch##width #= 1560.0;
    pitch##height #= 1200.0;
    pitch##anchor##set(0.333, 0.333);
    pitch##name #= "pitch";
    container##addChild(pitch) |> ignore;

    {
      application,
      container: container,
      state: None
    }
  };

  let loadFormation = (scene: pitchScene, ~formation: Formation.t, ~squad: Formation.squad) => {
    scene.container##children 
    /* null children appear on hot reload somehow */
    |. Utils.Array.optMap(element => !!element |. Js.Nullable.toOption)
    |. Belt.Array.forEach(element => {
      let name = (element##name |. Js.Nullable.toOption);
      
      if(name |. Belt.Option.getWithDefault("") |> Js.String.startsWith(Skin.names.playerMarkerPrefix) ||
        name |. Belt.Option.getWithDefault("") |> Js.String.startsWith(Skin.names.tacticRunArrow)){
        scene.container##removeChild(element) |> ignore;
      }
    });

    formation.elements
    |. Belt.Array.mapWithIndex((index, element) => {
      let gridPosition = gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y);
      
      switch(element.offensiveRun){
      | Some(offensiveRun) => {
        let offensiveRunTargetGrid = gridCellPosition(~withSize=40, ~x=offensiveRun.x, ~y=offensiveRun.y);
        let arrowTacticRun = Skin.offensiveRunArrow(
          ~x=gridPosition.x,
          ~y=gridPosition.y,
          ~tx=offensiveRunTargetGrid.x,
          ~ty=offensiveRunTargetGrid.y);
        scene.container##addChild(arrowTacticRun) |> ignore;
      } 
      | None => ()
      };

      switch(element.defensiveRun){
      | Some(defensiveRun) => {
        let defensiveRunTargetGrid = gridCellPosition(~withSize=40, ~x=defensiveRun.x, ~y=defensiveRun.y);
        let arrowTacticRun = Skin.defensiveRunArrow(
          ~x=gridPosition.x,
          ~y=gridPosition.y,
          ~tx=defensiveRunTargetGrid.x,
          ~ty=defensiveRunTargetGrid.y);
        scene.container##addChild(arrowTacticRun) |> ignore;
      }
      | None => ()
      };

      (index, element)
    })
    |. Belt.Array.forEach(((index, element)) => {
      let gridPosition = gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y);
      let playerMarker = Skin.playerMarker(
        ~x=gridPosition.x, ~y=gridPosition.y,
        ~number=squad[index].number,
        ~name=squad[index].name,
        ~position=element.position
      );
      
      scene.container##addChild(playerMarker) |> ignore;
    });

    {
      ...scene,
      state: Some({ formation, squad })
    }
  };

  let transitionTo = (scene: pitchScene, ~formation: Formation.t, ~labels: bool) => {
    let ease = Ease.ease;
    let subject: Rx.Subject.t(unit) = Rx.Subject.make();

    if(! !?scene.state){
      Console.warn("Transition has been called when no formation loaded, done nothing");
      Rx.Observable.of1(scene)
    } else {
      let state = ?!!scene.state;
      state.squad
      |. Belt.Array.mapWithIndex((idx, playerInfo) => {
        let number = playerInfo.number;
        (idx, scene.container##children |. Belt.Array.getBy(obj => obj##name |. Js.toOption == Some({j|marker:$number|j}))) })
      |. Belt.Array.forEach(((idx, container)) => {
        let element = formation.elements[idx];
        let gridPosition = gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y);

        switch(container){
        | Some(container) => { 
          ease |. Ease.add(
            ~element=container,
            ~params=`Point(Ease.easeParamsPoint(~x=gridPosition.x, ~y=gridPosition.y, ())),
            ~options=`EaseStringRepeatBool(Ease.addOptionsEaseStringRepeatBool(~duration=3000.0, ())),
            ()
          ) |> ignore;

          // ease |. Ease.add(
          //   ~element=((!!container)##children[1]),
          //   ~params=`Point(Ease.easeParamsPoint(~alpha=labels ? 1.0 : 0.0, ())),
          //   ~options=`EaseStringRepeatBool(Ease.addOptionsEaseStringRepeatBool(~duration=3000.0, ())),
          //   ()
          // ) |> ignore;

          ease |. EventEmitter.on(~event="complete", ~fn=_event => subject |. Rx.Subject.next(()), ()) |> ignore }
        | None => ease |. EventEmitter.on(~event="complete", ~fn=_event => subject |. Rx.Subject.next(()), ()) |> ignore }
      });

      subject 
        |. Rx.Subject.asObservable
        |> Rx.Observable.Operators.bufferCount(Array.length(state.squad))
        |> Rx.Observable.Operators.map(_entity => { ...scene, state: Some({ ...state, formation }) })
        |> Rx.Observable.Operators.take(1)
    }
  };
};