[%%debugger.chrome]

open Operators;
open Shortener;
open PIXI;
open PIXIExtras;
open Webapi;

module Const {
  let playerMarkerPrefix = "marker";
  let tacticRunArrow = "tacticRun";
};

module State {
  type formation = {
    formation: Formation.t,
    team: Team.t,
    compact: bool
  };

  type formationTeamVsTeam = {
    home: formation,
    away: formation
  };

  type t =
    | None
    | FormationSingle(formation)
    | FormationTeamVsTeam(formationTeamVsTeam);
}

type t = {
  application: Application.t,
  container: Viewport.t,
  skin: FormationSkin.t,
  state: State.t
};

/***
  * rescales and adjusts alpha according to zoom
  * independent of state: finds all marker and tactic run containers
  */
let handleZoom = (viewport: Viewport.t) => {

  viewport##children
  |. afilter(child => child##interactive)
  |. afilter(child =>
    child##name |. toopt |. withdef("") |> sstartswith(Const.playerMarkerPrefix))
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
  |. afilter(child => child##name |. toopt == Some(Const.tacticRunArrow))
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

/***
  * creates PIXI application with PIXI.Viewport as base container and pitch sprite
  */
let create = (~element: Dom.HtmlElement.t, ~skin: FormationSkin.t, ~width: int, ~height: int) => {
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
  let pitchTexture = Texture.from(~source=skin.pitchTexturePath);
  let pitch = Sprite.create(pitchTexture);
  pitch##width #= 1560.0;
  pitch##height #= 1200.0;
  pitch##anchor##set(0.333, 0.333);
  pitch##name #= "pitch";
  container##addChild(pitch) |> ignore;

  {
    application,
    container: container,
    skin,
    state: None
  }
};

/***
 * Remove all existing player marker and tactic arrows
 * Then loads player markers and tactic arrows
 */
let loadState = (scene: t, ~state: State.t) => {
  scene.container##children 
  /* null children appear on hot reload somehow */
  |. aoptmap(element => !!element |. toopt)
  |. afilter((element: DisplayObject.t) => 
    element##name |. toopt |. withdef("") |> sstartswith(Const.playerMarkerPrefix) ||
    element##name |. toopt |. withdef("") |> sstartswith(Const.tacticRunArrow))
  |. aforeach(element => 
    scene.container##removeChild(element) |> ignore
  );

  let _loadFormation = (state: State.formation) => {
    (state.compact ? state.formation.compact : state.formation.base)
    |. amapi((index, element) => {
      let gridPosition = Base.gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y, ~xoff=element.location.xOffset, ~yoff=element.location.yOffset);
      
      switch(element.offensiveRun){
      | Some(offensiveRun) => {
        let offensiveRunTargetGrid = Base.gridCellPosition(~withSize=40, ~x=offensiveRun.x, ~y=offensiveRun.y, ~xoff=offensiveRun.xOffset, ~yoff=offensiveRun.yOffset);
        let arrowTacticRun = scene.skin.offensiveRunArrow(
          ~x=gridPosition.x,
          ~y=gridPosition.y,
          ~tx=offensiveRunTargetGrid.x,
          ~ty=offensiveRunTargetGrid.y);
        arrowTacticRun##name #= Const.tacticRunArrow;
        scene.container##addChild(arrowTacticRun) |> ignore;
      } 
      | None => ()
      };

      switch(element.defensiveRun){
      | Some(defensiveRun) => {
        let defensiveRunTargetGrid = Base.gridCellPosition(~withSize=40, ~x=defensiveRun.x, ~y=defensiveRun.y, ~xoff=defensiveRun.xOffset, ~yoff=defensiveRun.yOffset);
        let arrowTacticRun = scene.skin.defensiveRunArrow(
          ~x=gridPosition.x,
          ~y=gridPosition.y,
          ~tx=defensiveRunTargetGrid.x,
          ~ty=defensiveRunTargetGrid.y);
        arrowTacticRun##name #= Const.tacticRunArrow;
        scene.container##addChild(arrowTacticRun) |> ignore;
      }
      | None => ()
      };

      (index, element)
    })
    |. aforeach(((index, element)) => {
      let gridPosition = Base.gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y, ~xoff=element.location.xOffset, ~yoff=element.location.yOffset);
      let playerMarker = scene.skin.playerMarker(
        ~x=gridPosition.x, ~y=gridPosition.y,
        ~number=state.team.squad[index].number,
        ~name=state.team.squad[index].name,
        ~position=element.position,
        ~colors=state.team.kit.home
      );

      let markerPrefix = Const.playerMarkerPrefix;
      let number = state.team.squad[index].number;
      let teamName = state.team.name;
      playerMarker##name #= {j|$markerPrefix:$teamName:$number|j};
      scene.container##addChild(playerMarker) |> ignore;
    });
  };

  switch(state){
  | None => ()
  | FormationSingle(state) => _loadFormation(state)
  | FormationTeamVsTeam(state) => {
    _loadFormation(state.home);
    _loadFormation({...state.away, formation: state.away.formation |. Formation.flipToAway(()) });
  }};

  {...scene, state}
};

let transitionToSkin = (scene: t, ~skin: FormationSkin.t) => {
  let ease = Ease.ease;
  let subject: Rx.Subject.t(unit) = Rx.Subject.make();

  let _teamTransition = (state: State.formation, skin: FormationSkin.t) => {
    state.team.squad
    |. amapi((idx, playerInfo) => {
      let number = playerInfo.number;
      let teamName = state.team.name;
      let markerPrefix = Const.playerMarkerPrefix;
      (idx, scene.container##children |. Belt.Array.getBy(obj => obj##name |. Js.toOption == Some({j|$markerPrefix:$teamName:$number|j}))) })
    |. aforeach(((idx, container)) => {
      let element = state.formation.base[idx];
      let playerInfo = state.team.squad[idx];

      switch(container){
      | Some(container) => {
        let newMarker = skin.playerMarker(
          ~x=container##x, ~y=container##y, 
          ~name=playerInfo.name, ~number=playerInfo.number,
          ~position=element.position,
          ~colors=state.team.kit.home);
        let markerPrefix = Const.playerMarkerPrefix;
        let number = playerInfo.number;
        let teamName = state.team.name;
        newMarker##name #= {j|$markerPrefix:$teamName:$number|j};

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
    })
  };

  switch(scene.state){
  | None => {
    Console.warn("Transition has been called when no formation loaded, done nothing");
    subject |. Rx.Subject.next(());
  }
  | FormationSingle(state) => _teamTransition(state, skin)
  | FormationTeamVsTeam(state) => {
    _teamTransition(state.home, skin);
    _teamTransition(state.away, skin);
  }};

  Rx.Observable.Operators.(
    subject
    |> Rx.Subject.asObservable
    |> bufferCount(
      switch(scene.state){
      | None => 1
      | FormationSingle(state) => state.team.squad|.alength
      | FormationTeamVsTeam(state) => (state.home.team.squad|.alength) + (state.away.team.squad|.alength)
      })
    |> map(_entity => { ...scene, skin })
    |> take(1))
};

let _formationTransition = (scene: t, ~team: Team.t, ~elements: array(Formation.element)) => {
  let ease = Ease.ease;
  let subject: Rx.Subject.t(unit) = Rx.Subject.make();
  let teamName = team.name;

  team.squad
  |. amapi((idx, playerInfo) => {
    let number = playerInfo.number;
    (idx, scene.container##children |. Belt.Array.getBy(obj => obj##name |. Js.toOption == Some({j|marker:$teamName:$number|j}))) })
  |. aforeach(((idx, container)) => {
    let element = elements[idx];
    let gridPosition = Base.gridCellPosition(~withSize=40, ~x=element.location.x, ~y=element.location.y, ~xoff=element.location.xOffset, ~yoff=element.location.yOffset);

    switch(container){
    | Some(container) => { 
      ease |. Ease.add(
        ~element=container,
        ~params=`Point(Ease.easeParamsPoint(~x=gridPosition.x, ~y=gridPosition.y, ())),
        ~options=`EaseStringRepeatBool(Ease.addOptionsEaseStringRepeatBool(~duration=3000.0, ())),
        ()
      ) |> ignore;

      ease |. EventEmitter.on(~event="complete", ~fn=_event => subject |. Rx.Subject.next(()), ()) |> ignore 
    }
    | None => subject |. Rx.Subject.next(()) }
  });

  Rx.Observable.Operators.(
    subject 
    |. Rx.Subject.asObservable
    |> bufferCount(Array.length(team.squad))
    |> map(_entity => ())
    |> take(1))
};

let _singleFormationTransition = (scene: t, ~compact: bool) =>
  switch(scene.state){
  | None => {
    Console.warn("Transition has been called when no formation loaded, done nothing");
    Rx.Observable.of1(scene)
  }
  | FormationSingle(state) => {
    if(state.compact == compact){
      Console.warn("Formation is already in compact mode, done nothing");
      Rx.Observable.of1(scene)
    } else {
      scene 
      |. _formationTransition(~team=state.team, ~elements=compact ? state.formation.compact : state.formation.base)
      |> Rx.Observable.Operators.map(() => {
        ...scene,
        state: FormationSingle({...state, compact })
      })
    }
  }
  | FormationTeamVsTeam(_state) => {
    Console.warn("Transition to compact is only supported in single formation mode");
    Rx.Observable.of1(scene);
  }};


let transitionToCompact = _singleFormationTransition(~compact=true);
let transitionToBase = _singleFormationTransition(~compact=false);

let twoTeamsFormationTransition = (scene: t, ~home: array(Formation.element), ~away: array(Formation.element)) => 
  switch(scene.state){
  | FormationTeamVsTeam(state) => {
    Rx.Observable.merge2(
      scene |. _formationTransition(~team=state.home.team, ~elements=home),
      scene |. _formationTransition(~team=state.away.team, ~elements=away |. Formation.flipElements(()))) 
    |> Rx.Observable.Operators.bufferCount(2)
    |> Rx.Observable.Operators.take(1)
    |> Rx.Observable.Operators.map(_value => scene)
  }
  | _ => Rx.Observable.of1(scene)
  };