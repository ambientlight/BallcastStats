module List {
  let join = (~seperator, list) => 
    List.fold_left((a, b) => a ++ seperator ++ b, "", list);
};

module Array {
  let optMap = (array: array('a), project: 'a => option('b)): array('b) => 
    array
    |. Belt.Array.map(project)
    |. Belt.Array.keep(element => element |. Belt.Option.isSome)
    |. Belt.Array.map(element => element |. Belt.Option.getExn)
}

module Rx {
  let optMap = (project, observable) => Rx.Operators.({
    observable
    |> map((x, _idx) => project(x))
    |> filter((entry, _idx) => Belt.Option.isSome(entry))
    |> map((entry, _idx) => Belt.Option.getExn(entry))
  });
};

module Tests {
  let withTestClass = (className, testClassName) =>
    List.join(~seperator=" ", [className, testClassName]);
};

module Option {
  let optMap = Belt.Option.map;
}