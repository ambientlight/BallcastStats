let (|?) = (opt, default) => Belt.Option.getWithDefault(opt, default);
let (!?) = opt => opt == None ? false : true;
let (>|<) = (list, seperator) => Utils.List.join(~seperator, list);
let (~~) = anything => Console.log(anything);
let (!!) = anything => Obj.magic(anything);
let (!#) = rule => Css.important(rule);
let (??) = value => Some(value);