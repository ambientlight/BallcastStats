type position = 
  | GK
  | CB
  | LB
  | RB
  | LWB
  | RWB
  | DM
  | CM
  | AM
  | LM
  | RM
  | LW
  | RW
  | CF
  | ST;

type gridLocation = {
  x: int,
  y: int,
  xOffset: float,
  yOffset: float
};

type element = {
  position: position,
  location: gridLocation
};

type t = {
  name: string,
  elements: array(element)
};