open Shortener
open Operators
open ReductiveRendererCore

let toFormation = (~formation: Formation.t, ~st: int, ~ed: int, ~mode: Team.View.layoutMode, ~pitchBounds: Core.Bounds.t, teamView: Object.group): Object.group => {
  // resort formation layout by order (just in case)
  let layout = formation.layout
  |. Team.View.homeTo(~mode, ~gridBounds={x: pitchBounds.br.x, y: pitchBounds.br.y})
  |> Array.to_list
  |> List.sort((lhs: Formation.element, rhs: Formation.element) => {
    Formation.orderValue(lhs.position) - Formation.orderValue(rhs.position)
  });

  let markers = teamView.obj
  |> Array.to_list;

  let eventReplacedMarkers = lzip(layout, markers)
  |> lmap(((elem, marker): (Formation.element, Object.t)) => {
    let newTransition = TimelineEvent.Make.positionTransition(~st, ~ed, ~pos={x:elem.location.x -. 1.25, y:elem.location.y -. 2.25}) |> TimelineEvent.lift;
    let rend = Object.renderable(marker);
    marker |> Object.withRenderableTimeline({
      ...rend.timeline, 
      events: switch(rend.timeline.events){
      | `Some(events) => `Some(events |@ [|newTransition|])
      | `None() => `Some([|newTransition|])
      }})
  })
  |> Array.of_list;

  {...teamView, obj: eventReplacedMarkers }
};