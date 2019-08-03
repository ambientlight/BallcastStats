open Operators;
open Webapi;
open PIXI;
open PIXIExtras;
open Shortener;

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
  team: Team.t
};

type pitchScene = {
  application: Application.t,
  container: Viewport.t,
  state: option(state)
};

module type PitchScene {
  let create: (~element: Dom.HtmlElement.t, ~width: int, ~height: int) => pitchScene;
  let handleZoom: Viewport.t => unit;
  let loadFormation: (pitchScene, ~formation: Formation.t, ~team: Team.t) => pitchScene;
  
  let transitionToFormation: (pitchScene, ~formation: Formation.t, ~labels: bool) => Rx.Observable.t(pitchScene);
  let transitionToSkin: (pitchScene, ~skinBundle: FormationSkin.generators) => Rx.Observable.t(pitchScene);
};

module BuildPitchScene = (Skin: FormationSkin.T): PitchScene => {
  let handleZoom = (viewport: Viewport.t) => {

    viewport##children
    |. afilter(child => child##interactive)
    |. afilter(child =>
      child##name |. toopt |. withdef("") |> sstartswith(Skin.names.playerMarkerPrefix))
    |. amap(child => { let container: Container.t = !!child; container })
    |. aforeach(child => {
      let lastViewport = (viewport##lastViewport |. utoopt) |? [%bs.obj {
        scaleX: viewport##scale##x,
        scaleY: viewport##scale##y,
        x: 0.0,
        y: 0.0
      }];
      child##width #= (40.0 /. (lastViewport##scaleX));
      child##height #= (48.0 /. (lastViewport##scaleY));
    });

    viewport##children
    |. afilter(child => child##name |. toopt == Some(Skin.names.tacticRunArrow))
    |. amap(child => { let container: Container.t = !!child; container })
    |. aforeach(child => {
      let lastViewport = (viewport##lastViewport |. utoopt) |? [%bs.obj {
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

    container 
    |. EventEmitter.on(~event="zoomed", ~fn=(event: Js.t({.. viewport: Viewport.t})) => handleZoom(event##viewport), ())
    |. EventEmitter.on(~event="zoomed-end", ~fn=(viewport: Viewport.t) => (), ())
    |> ignore;

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

  let loadFormation = (scene: pitchScene, ~formation: Formation.t, ~team: Team.t) => {
    scene.container##children 
    /* null children appear on hot reload somehow */
    |. aoptmap(element => !!element |. toopt)
    |. afilter((element: DisplayObject.t) => 
      element##name |. toopt |. withdef("") |> sstartswith(Skin.names.playerMarkerPrefix) ||
      element##name |. toopt |. withdef("") |> sstartswith(Skin.names.tacticRunArrow))
    |. aforeach(element => 
      scene.container##removeChild(element) |> ignore
    );

    formation.elements
    |. amapi((index, element) => {
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
    |. aforeach(((index, element)) => {
      let gridPosition = gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y);
      let playerMarker = Skin.playerMarker(
        ~x=gridPosition.x, ~y=gridPosition.y,
        ~number=team.squad[index].number,
        ~name=team.squad[index].name,
        ~position=element.position,
        ~colors=team.kit.home
      );

      let markerPrefix = Skin.names.playerMarkerPrefix;
      let number = team.squad[index].number;
      playerMarker##name #= {j|$markerPrefix:$number|j};
      scene.container##addChild(playerMarker) |> ignore;
    });

    {
      ...scene,
      state: Some({ formation, team })
    }
  };

  let transitionToSkin = (scene: pitchScene, ~skinBundle: FormationSkin.generators) => {
    let ease = Ease.ease;
    let subject: Rx.Subject.t(unit) = Rx.Subject.make();

    if(! !?scene.state){
      Console.warn("Transition has been called when no formation loaded, done nothing");
      Rx.Observable.of1(scene)
    } else {
      let state = ?!!scene.state;
      state.team.squad
      |. amapi((idx, playerInfo) => {
        let number = playerInfo.number;
        (idx, scene.container##children |. Belt.Array.getBy(obj => obj##name |. Js.toOption == Some({j|marker:$number|j}))) })
      |. aforeach(((idx, container)) => {
        let element = state.formation.elements[idx];
        let playerInfo = state.team.squad[idx];

        switch(container){
        | Some(container) => {
          let newMarker = skinBundle.playerMarker(
            ~x=container##x, ~y=container##y, 
            ~name=playerInfo.name, ~number=playerInfo.number,
            ~position=element.position,
            ~colors=state.team.kit.home);
          let markerPrefix = Skin.names.playerMarkerPrefix;
          let number = playerInfo.number;
          newMarker##name #= {j|$markerPrefix:$number|j};

          let twidth = newMarker##width;
          let theight = newMarker##height;
          let talpha = newMarker##alpha;
          
          newMarker##width #= 0.0;
          newMarker##height #= 0.0;
          newMarker##alpha #= 0.0;
          scene.container##addChild(newMarker) |> ignore;

          ease |. Ease.add(
            ~element=container, 
            ~params=`Point(Ease.easeParamsPoint(~width=0.0, ~height=0.0, ~alpha=0.0, ())),
            ~options=`EaseStringRepeatBool(Ease.addOptionsEaseStringRepeatBool(~duration=1000.0, ())),
            ()) |> ignore;

          ease |. Ease.add(
            ~element=newMarker,
            ~params=`Point(Ease.easeParamsPoint(~width=twidth, ~height=theight, ~alpha=talpha, ())),
            ~options=`EaseStringRepeatBool(Ease.addOptionsEaseStringRepeatBool(~duration=1000.0, ())),
            ()) |> ignore;

          ease |. EventEmitter.on(~event="complete", ~fn=_event => {
            scene.container##removeChild(container) |> ignore;
            subject |. Rx.Subject.next(());
          }, ()) |> ignore
        }
        | None => ease |. EventEmitter.on(~event="complete", ~fn=_event => subject |. Rx.Subject.next(()), ()) |> ignore
        }
      });

      Rx.Observable.Operators.(
        subject
        |> Rx.Subject.asObservable
        |> bufferCount(Array.length(state.team.squad))
        |> map(_entity => scene)
        |> take(1))
    };
  };

  let transitionToFormation = (scene: pitchScene, ~formation: Formation.t, ~labels: bool) => {
    let ease = Ease.ease;
    let subject: Rx.Subject.t(unit) = Rx.Subject.make();

    if(! !?scene.state){
      Console.warn("Transition has been called when no formation loaded, done nothing");
      Rx.Observable.of1(scene)
    } else {
      let state = ?!!scene.state;
      state.team.squad
      |. amapi((idx, playerInfo) => {
        let number = playerInfo.number;
        (idx, scene.container##children |. Belt.Array.getBy(obj => obj##name |. Js.toOption == Some({j|marker:$number|j}))) })
      |. aforeach(((idx, container)) => {
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

          ease |. EventEmitter.on(~event="complete", ~fn=_event => subject |. Rx.Subject.next(()), ()) |> ignore 
        }
        | None => ease |. EventEmitter.on(~event="complete", ~fn=_event => subject |. Rx.Subject.next(()), ()) |> ignore }
      });

      Rx.Observable.Operators.(
        subject 
        |. Rx.Subject.asObservable
        |> bufferCount(Array.length(state.team.squad))
        |> map(_entity => { ...scene, state: Some({ ...state, formation }) })
        |> take(1))
    }
  };
};