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

type gridLocation = {
  x: int,
  y: int,
  xOffset: float,
  yOffset: float
};

type element = {
  position: position,
  location: gridLocation,
  offensiveRun: option(gridLocation),
  defensiveRun: option(gridLocation)
};

type t = {
  name: string,
  base: array(element),
  compact: array(element)
};

let flipElements = (elements: array(element), ~maxCellX = 13, _unit) =>
  elements 
  |. amap(element => {...element, location: {...element.location, x: maxCellX - element.location.x}});

let flipToAway = (formation: t, ~maxCellX = 13, _unit) => {
  ...formation,
  base: formation.base |. flipElements(~maxCellX, ()),
  compact: formation.compact |. flipElements(~maxCellX, ())
};

let normal_4132: t = {
  name: "4-1-3-2",
  base: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 4, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 9, y: 2, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 2, y: 3, xOffset: 0.0, yOffset: 0.0}) },
    { position: `CB, location: { x: 3, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 5, y: 4, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `CB, location: { x: 3, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 5, y: 6, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `RB, location: { x: 4, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 9, y: 8, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 2, y: 7, xOffset: 0.0, yOffset: 0.0}) },
    { position: `DM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 7, y: 5, xOffset: 0.0, yOffset: 0.0}), defensiveRun: Some({ x: 3, y: 5, xOffset: 0.0, yOffset: 0.0}) },
    { position: `CM, location: { x: 7, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 8, y: 4, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 4, y: 3, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `CM, location: { x: 7, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: Some({ x: 8, y: 6, xOffset: 0.0, yOffset: 0.0 }), defensiveRun: Some({ x: 4, y: 7, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `AM, location: { x: 9, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 6, y: 5, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `LW, location: { x: 11, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 6, y: 2, xOffset: 0.0, yOffset: 0.0 }) },
    { position: `RW, location: { x: 11, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: Some({ x: 6, y: 8, xOffset: 0.0, yOffset: 0.0 }) }
  |],

  compact: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 2, y: 3, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 2, y: 7, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 3, y: 5, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `AM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 6, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 6, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
  |]
};

let liv_433: t = {
  name: "4-3-3",
   base: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 4, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 3, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 3, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 4, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 7, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 7, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `ST, location: { x: 11, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 10, y: 3, xOffset: 0.0, yOffset: -20.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 10, y: 7, xOffset: 0.0, yOffset: 20.0 }, offensiveRun: None, defensiveRun: None }
  |],

  compact: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 2, y: 2, xOffset: 20.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 2, y: 8, xOffset: 20.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 3, y: 5, xOffset: 20.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `ST, location: { x: 6, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 6, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 6, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
  |]
};

let bar_433: t = {
  name: "4-3-3",
   base: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 3, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 3, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 3, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 3, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 7, y: 3, xOffset: 20.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 7, y: 7, xOffset: 20.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `ST, location: { x: 11, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 11, y: 3, xOffset: 0.0, yOffset: -10.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 11, y: 7, xOffset: 0.0, yOffset: 10.0 }, offensiveRun: None, defensiveRun: None }
  |],

  compact: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 2, y: 2, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 2, y: 8, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 3, y: 5, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `ST, location: { x: 6, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 6, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 6, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
  |]
};

let test_def = [|
  { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
  { position: `LB, location: { x: 2, y: 3, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
  { position: `CB, location: { x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
  { position: `CB, location: { x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
  { position: `RB, location: { x: 2, y: 7, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
  { position: `DM, location: { x: 3, y: 5, xOffset: -20.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
  { position: `CM, location: { x: 4, y: 3, xOffset: -20.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
  { position: `CM, location: { x: 4, y: 7, xOffset: -20.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
  { position: `AM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
  { position: `LW, location: { x: 5, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
  { position: `RW, location: { x: 5, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
|];

let test_off = [|
    { position: `GK, location: { x: 2, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 9, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 5, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 5, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 9, y: 8, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 7, y: 5, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 8, y: 4, xOffset: 0.0, yOffset: -20.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 8, y: 6, xOffset: 0.0, yOffset: 20.0 }, offensiveRun: None, defensiveRun: None },
    { position: `AM, location: { x: 9, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 10, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 10, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
  |];

/*
let single_defensive_4132: t = {
  name: "4-1-3-2",
  elements: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 2, y: 3, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 2, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 2, y: 7, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 3, y: 5, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 4, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `AM, location: { x: 5, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 6, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 6, y: 8, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
  |]
};

let single_offensive_4132: t = {
  name: "4-1-3-2",
  elements: [|
    { position: `GK, location: { x: 1, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LB, location: { x: 9, y: 2, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 5, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CB, location: { x: 5, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RB, location: { x: 9, y: 8, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `DM, location: { x: 7, y: 5, xOffset: 0.0, yOffset: 0.0}, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 8, y: 4, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `CM, location: { x: 8, y: 6, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `AM, location: { x: 9, y: 5, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `LW, location: { x: 11, y: 3, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None },
    { position: `RW, location: { x: 11, y: 7, xOffset: 0.0, yOffset: 0.0 }, offensiveRun: None, defensiveRun: None }
  |]
};
*/