open Webapi;
open Css;
open Shortener;

module Styles {
  let root = style([
    display(`flex),
    alignItems(`center),
    justifyContent(`spaceAround),
    flexDirection(`column),

    width(vw(100.0)),
    height(vh(100.0)),

    backgroundColor(hsla(133, 51, 10, 1.0))
  ])
};

module EmbedRenderer {
  open ReductiveRendererCore;

  let devToolsEnhancer =
    ReductiveDevTools.Connectors.enhancer(
      ~options=ReductiveDevTools.Extension.enhancerOptions(
        ~name="FormationEmbedCore",
        //rougly 2-actions per frame (tick + update) * 10 seconds (at 60 fps)
        ~maxAge=60 * 2 * 10,
        ~actionCreators={
          "startTicker": () => Action.StartTicker(()) |. ReductiveDevTools.Utilities.Serializer.serializeAction,
          "endTicker": () => Action.EndTicker(()) |. ReductiveDevTools.Utilities.Serializer.serializeAction,
          "setFrame": frame => Action.SetFrame(frame) |. ReductiveDevTools.Utilities.Serializer.serializeAction
        },
        ~latency=0,
        ()),
      ~devToolsUpdateActionCreator=(devToolsState: State.t) => Action.DevToolsUpdate(devToolsState),
      ());

  let store = appContainer => Store.createRendererStore(
    (module ReductiveRendererPIXI.Enhancer), 
    (module ReductiveRendererPIXI.DefaultResolvable),
    ~appContainer,
    ~enhancer=devToolsEnhancer, 
    ());

  let teamViewExpansion: (
    Team.View.t,
    ~marker: Object.t,
    ~bounds: Core.Bounds.t,
    ~props: Renderable.props=?,
    ~tags: Core.StringMap.t(Core.Tag.t)=?,
    ~timeline:TimelineEvent.presence=?,
    unit
  ) => Object.group = (view, ~marker, ~bounds, ~props=Renderable.defaultProperties, ~tags=Core.StringMap.empty, ~timeline=TimelineEvent.Make.presence(), ()) => {

    let squadLength = alength(view.squad);
    let formationLength = alength(view.formation.layout);
    if(squadLength != formationLength){
      {j|SetTeamView has been passed with squad($squadLength)/formation($formationLength) not equal in length.|j} -> Console.warn;
    };
    
    let formation = view.formation.layout 
      |. Team.View.homeTo(~mode=view.mode, ~gridBounds={x: bounds.br.x, y: bounds.br.y});        

    let markers = azip(view.squad, formation)
    |. amap(((player, element)) => {
      marker
      |> Object.withId(IdGen.id())
      |> Object.withRenderableProps({ 
        ...Object.renderable(marker).props, 
        p: { x: element.location.x -. 1.25, y: element.location.y -. 2.25 }
      });
    });

    Object.Make.group(~objs=markers, ())
  }
};

module EmbedContainer {
  open ReductiveRendererCore;

  let embedWidth = 328.;
  let embedHeight = 490.;

  [@bs.module] external fieldModel: string = "assets/svgs/field.svg";
  [@bs.module] external fieldPattern: string = "assets/svgs/field_pattern.svg";
  [@bs.module] external playerMarker: string = "assets/svgs/player_marker.svg";

  let coordSys: CoordSystem.t = {
    cell: { width: 40., height: 40. },
    bounds: { 
      ul: { x: 0., y: 0. },
      br: { x: 13., y: 10. } 
    },
    insets: {
      left: 0.4,
      top: 1.0,
      right: 12.6,
      bottom: 9.0 
    },
    orientation: CoordSystem.Vertical
  };

  let pitchBounds: Core.Bounds.t = { ul: { x: 0., y: 0. }, br: { x: 13., y: 10. }};

  let playerMarker = SvgLoader.load(playerMarker, ~containerSize={ width: embedWidth , height: embedHeight }, ~grid=coordSys);
  let liverpoolView = Team.View.create(
    ~team=Team.liverpool,
    ~formation=Formation.normal_4132,
    ~kit=Team.Kit.Home,
    ~mode=Team.View.Away)
  |. EmbedRenderer.teamViewExpansion(
    ~marker=playerMarker,
    ~bounds=pitchBounds,
    ())
  |> Transition.toFormation(~st=10, ~ed=70, ~formation=Formation.bar_433, ~mode=Team.View.Away, ~pitchBounds)
  |. Object.Group;

  let pitchModelDoc: Document.t = {
    objects: Object.Group(Object.Make.group(~objs=[|
      SvgLoader.load(fieldPattern, ~containerSize={ width: embedWidth , height: embedHeight }, ~grid=coordSys),
      SvgLoader.load(fieldModel, ~containerSize={ width: embedWidth , height: embedHeight }, ~grid=coordSys),
      liverpoolView
    |], ())),
    coordSys,
    meta: Core.StringMap.empty,
    skin: ""
  };

  module Styles {
    let root = style([
      height(px(int_of_float(embedHeight) + 2)),
      width(px(int_of_float(embedWidth) + 2)),
      border(px(1), `solid, rgba(255,255,255,0.25)),
      backgroundColor(rgba(255, 255, 255, 0.15))
    ]);
  };

  [@react.component]
  let make = () => {
    let pixiContainerRef: React.Ref.t(Js.nullable(Dom.Element.t)) = React.useRef(Js.Nullable.null);
    let storeRef: React.Ref.t(option(Reductive.store(ReductiveRendererCore.Action.t, ReductiveRendererCore.State.t))) = React.useRef(None);

    React.useEffect(() => {
      switch(storeRef |. React.Ref.current, pixiContainerRef |. React.Ref.current |. Js.Nullable.toOption ){
      | (None, Some(pixiContainer)) => {
        let embedStore = EmbedRenderer.store(pixiContainer);
        embedStore |. Reductive.Store.dispatch(Action.SetDocument(pitchModelDoc));
        embedStore |. Reductive.Store.dispatch(Action.StartTicker());
        storeRef |. React.Ref.setCurrent(Some(embedStore));
        None
      }
      | _ => None
      }
    });

    <div className=Styles.root ref={ReactDOMRe.Ref.domRef(pixiContainerRef)}>
    </div>
  };
}

[@react.component]
let make = () => 
  <div className=Styles.root>
    <EmbedContainer/>
  </div>