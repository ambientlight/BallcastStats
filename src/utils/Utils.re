module List {
  let join = (~seperator, list) => 
    List.fold_left((a, b) => a ++ seperator ++ b, "", list);
};

module Rx {
  let optMap = (project, observable) => Rx.Observable.Operators.({
    observable
    |> map(project)
    |> filter(entry => Belt.Option.isSome(entry))
    |> map(entry => Belt.Option.getExn(entry))
  });
};

module Tests {
  let withTestClass = (className, testClassName) =>
    List.join(~seperator=" ", [className, testClassName]);
};