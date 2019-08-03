open Operators;
open Css;
open Webapi;
open Rx.Observable.Operators;

let gridCellSize = 20;
let embedWidth = 520;
let embedHeight = 400;

module Styles {
  let root = style([
    height(px(embedHeight + 2)),
    width(px(embedWidth + 2)),
    border(px(1), `solid, rgba(255,255,255,0.2))
  ])
};

type retained = {
  mutable pixiContainerRef: option(Dom.HtmlElement.t)
};

type state = {
  scene: option(PitchScene.t)
}

type action = SetScene(option(PitchScene.t));

let component = ReasonReact.reducerComponentWithRetainedProps(__MODULE__);
let make = (_children) => {
  ...component,
  retainedProps: {
    pixiContainerRef: None,
  },

  initialState: () => {
    scene: None
  },

  reducer: (action, state) => switch(action){
  | SetScene(scene) => ReasonReact.Update({ 
    ...state,
    scene
  })
  },

  didMount: self => {
    let pixiContainerRef = self.retainedProps.pixiContainerRef;
    Rx.Observable.merge([|
      FontFaceObserver.create(~fontFamily="Gobold", ()) |> FontFaceObserver.load |> Rx.Observable.fromPromise,
      FontFaceObserver.create(~fontFamily="Jost", ~options=FontFaceObserver.options(~weight=800, ()), ()) |> FontFaceObserver.load |> Rx.Observable.fromPromise
    |])
    |> Rx.Observable.Operators.bufferCount(2)
    |> Rx.Observable.Operators.take(1)
    |> Rx.Observable.subscribe(~next=_value =>
      Belt.Option.map(pixiContainerRef, containerRef => {
        let scene = PitchScene.create(
          ~element=containerRef, ~skin=FormationSkin.Base.bundle,
          ~width=embedWidth, ~height=embedHeight);
        self.send(SetScene(Some(scene)));
      }) |> ignore
    ) |> ignore
  },

  didUpdate: ({ oldSelf, newSelf }) => {
    newSelf.state.scene 
    |. Belt.Option.map(scene => { 
      let scene = scene |. PitchScene.loadState(
        ~state=PitchScene.State.FormationSingle({
          formation: Formation.normal_4132,
          team: Team.barcelona,
          compact: false
        }),
      );
      
      /**
       * make sure all content is properly adjusted to a current zoom 
       * this is important during hot reload
       */
      PitchScene.handleZoom(scene.container);
      Rx.Observable.of1(scene)
      |> delay(5000)
      |> mergeMap(scene => scene |> PitchScene.transitionToSkin(~skin=FormationSkin.Compact.bundle))
      |> mergeMap(scene => scene |> PitchScene.transitionToCompact)
      |> mergeMap(scene => scene |> PitchScene.transitionToBase)
      |> Rx.Observable.subscribe(~complete=() => ());
    })
    |> ignore;
  },

  render: ({ retainedProps }) => 
    <div className=Styles.root ref=(element => { retainedProps.pixiContainerRef = Js.Nullable.toOption(!!element) })>
    </div>
};