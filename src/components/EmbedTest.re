open Webapi;
open Css;

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
};

module EmbedContainer {
  open ReductiveRendererCore;

  let embedWidth = 328.;
  let embedHeight = 490.;

  [@bs.module] external fieldModel: string = "assets/svgs/field.svg";
  [@bs.module] external fieldPattern: string = "assets/svgs/field_pattern.svg";

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

  let pitchModelDoc: Document.t = {
    objects: Object.Group(Object.Make.group(~objs=[|
      SvgLoader.load(fieldPattern, ~containerSize={ width: embedWidth , height: embedHeight }, ~grid=coordSys),
      SvgLoader.load(fieldModel, ~containerSize={ width: embedWidth , height: embedHeight }, ~grid=coordSys),
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
        let store = EmbedRenderer.store(pixiContainer);
        store |. Reductive.Store.dispatch(Action.SetDocument(pitchModelDoc));
        storeRef |. React.Ref.setCurrent(Some(store));
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