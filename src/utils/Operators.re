let (|?) = (opt, default) => Belt.Option.getWithDefault(opt, default);
let (>|<) = (list, seperator) => Utils.List.join(~seperator, list);
let (~~) = anything => Console.log(anything);
let (!!) = anything => Obj.magic(anything);