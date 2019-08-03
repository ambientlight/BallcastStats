type position = {
  x: float,
  y: float
};

let gridCellPosition = (~withSize as size: int, ~x: int, ~y: int, ~xoff: float, ~yoff: float): position => {
  x: float(x * size) +. xoff,
  y: float(y * size) +. yoff
};
