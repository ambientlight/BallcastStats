open Shortener;

[@bs.deriving jsConverter]
type position = [
  | `GK
  | `CB
  | `LB
  | `RB
  | `LWB
  | `RWB
  | `DM
  | `CM
  | `AM
  | `LM
  | `RM
  | `LW
  | `RW
  | `CF
  | `ST];

let orderValue: position => int = fun
  | `GK => 0
  | `LB => 1
  | `CB => 2
  | `RB => 3
  | `LWB => 4
  | `RWB => 5
  | `DM => 6
  | `CM => 7
  | `LM => 8
  | `RM => 9
  | `AM => 10
  | `LW => 11
  | `RW => 12
  | `CF => 13
  | `ST => 14;

type element = {
  position: position,
  location: ReductiveRendererCore.Grid.position
};

type t = {
  name: string,
  layout: array(element)
};

let normal_4132: t = {
  name: "4-1-3-2",
  layout: [|
    { position: `GK, location: { x: 1., y: 5. }},
    { position: `LB, location: { x: 4., y: 2. }},
    { position: `CB, location: { x: 3., y: 4. }},
    { position: `CB, location: { x: 3., y: 6. }},
    { position: `RB, location: { x: 4., y: 8. }},
    { position: `DM, location: { x: 5., y: 5. }},
    { position: `CM, location: { x: 7., y: 3. }},
    { position: `CM, location: { x: 7., y: 7. }},
    { position: `AM, location: { x: 9., y: 5. }},
    { position: `LW, location: { x: 11., y: 3. }},
    { position: `RW, location: { x: 11., y: 7. }}
  |]
};

let liv_433: t = {
  name: "4-3-3",
   layout: [|
    { position: `GK, location: { x: 1., y: 5. }},
    { position: `LB, location: { x: 4., y: 2. }},
    { position: `CB, location: { x: 3., y: 4. }},
    { position: `CB, location: { x: 3., y: 6. }},
    { position: `RB, location: { x: 4., y: 8. }},
    { position: `DM, location: { x: 5., y: 5. }},
    { position: `CM, location: { x: 7., y: 3. }},
    { position: `CM, location: { x: 7., y: 7. }},
    { position: `ST, location: { x: 11., y: 5. }},
    { position: `LW, location: { x: 10., y: 3. }},
    { position: `RW, location: { x: 10., y: 7. }}
  |],
};

let bar_433: t = {
  name: "4-3-3",
   layout: [|
    { position: `GK, location: { x: 1., y: 5., }},
    { position: `LB, location: { x: 3., y: 2., }},
    { position: `CB, location: { x: 3., y: 4., }},
    { position: `CB, location: { x: 3., y: 6., }},
    { position: `RB, location: { x: 3., y: 8., }},
    { position: `DM, location: { x: 5., y: 5., }},
    { position: `CM, location: { x: 7., y: 3., }},
    { position: `CM, location: { x: 7., y: 7., }},
    { position: `ST, location: { x: 11., y: 5., }},
    { position: `LW, location: { x: 11., y: 3., }},
    { position: `RW, location: { x: 11., y: 7., }}
  |]
};

let def_451: t = {
  name: "4-5-1",
  layout: [|
    { position: `GK, location: { x: 1., y: 5., }},
    { position: `LB, location: { x: 2.5, y: 2., }},
    { position: `CB, location: { x: 2.5, y: 4., }},
    { position: `CB, location: { x: 2.5, y: 6., }},
    { position: `RB, location: { x: 2.5, y: 8., }},
    { position: `DM, location: { x: 4.5, y: 4., }},
    { position: `DM, location: { x: 4.5, y: 6., }},
    { position: `CM, location: { x: 6.5, y: 5., }},
    { position: `LM, location: { x: 8., y: 2.5, }},
    { position: `RM, location: { x: 8., y: 7.5, }},
    { position: `CF, location: { x: 11., y: 5., }}
  |]
};

let dummy = (location: ReductiveRendererCore.Grid.position, name: string) => {
  name,
  layout: [|
    { position: `GK, location},
    { position: `LB, location},
    { position: `CB, location},
    { position: `CB, location},
    { position: `RB, location},
    { position: `DM, location},
    { position: `DM, location},
    { position: `CM, location},
    { position: `LM, location},
    { position: `RM, location},
    { position: `CF, location}
  |]
};