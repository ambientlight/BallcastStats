open Css;

module Breakpoint {
  type t = 
    | SmallPhone
    | Phone
    | Tablet
    | Laptop
    | Desktop
    | FullHD;

  let toString = fun
    | SmallPhone => "SmallPhone"
    | Phone => "Phone"
    | Tablet => "Tablet"
    | Laptop => "Laptop"
    | Desktop => "Desktop"
    | FullHD => "FullHD";

  let range = fun
    | SmallPhone => (0, 360)
    | Phone => (360, 600)
    | Tablet => (600, 1024)
    | Laptop => (1024, 1440)
    | Desktop => (1440, 1920)
    | FullHD => (1920, Js.Int.max);
  
  let rangeL = breakpoint => { let (lower, _) = range(breakpoint); lower };
  let rangeU = breakpoint => { let (_, upper) = range(breakpoint); upper };

  let matKey = fun
    | SmallPhone
    | Phone => "xs"
    | Tablet => "sm"
    | Laptop => "md"
    | Desktop => "lg"
    | FullHD => "xl";

  let current = () => switch(Window.innerWidth){
    | width when width < rangeU(SmallPhone) => SmallPhone
    | width when width >= rangeL(Phone) && width < rangeU(Phone) => Phone
    | width when width >= rangeL(Tablet) && width < rangeU(Tablet) => Tablet
    | width when width >= rangeL(Laptop) && width < rangeU(Laptop) => Laptop
    | width when width >= rangeL(Desktop) && width < rangeU(Desktop) => Desktop
    | _ => FullHD;
  }

  let logResize = () => 
    Window.addEventListener(`resize(_event => {
      let current = current() |> toString;
      let width = Window.innerWidth;
      Js.log({j|$width: $current|j});
    }));
};

let atLeast = (breakpoint: Breakpoint.t, rules: list(Css.rule)) => {
  let lower = Breakpoint.rangeL(breakpoint);
  media({j|(min-width: $(lower)px)|j}, rules)
}

let atMost = (breakpoint: Breakpoint.t, rules: list(Css.rule)) => {
  let upperMinus1 = Breakpoint.rangeU(breakpoint) - 1;
  media({j|(max-width: $(upperMinus1).9px)|j}, rules)
}

let exactly = (breakpoint: Breakpoint.t, rules: list(Css.rule)) => {
  let (lower, upper) = Breakpoint.range(breakpoint);
  let upperMinus1 = upper - 1;
  media({j|(min-width: $(lower)px) and (max-width: $(upperMinus1).9px)|j}, rules)
}

let between = (lower: Breakpoint.t, upper: Breakpoint.t, rules: list(rule)) => {
  let lowerRange = Breakpoint.rangeU(upper) <= Breakpoint.rangeL(lower) ? upper : lower;
  let upperRange = Breakpoint.rangeU(upper) <= Breakpoint.rangeL(lower) ? lower : upper;
  
  if(Breakpoint.rangeU(upper) <= Breakpoint.rangeL(lower)){
    let lowerStr = lower |> Breakpoint.toString;
    let upperStr = upper |> Breakpoint.toString;
    Console.warn({j|Breakpoint range order is incorrect: $lowerStr is above $upperStr. order has been reversed|j});
    Console.trace("Breakpoint range order is incorrect: Trace")
  };

  let targetLower = lowerRange |> Breakpoint.rangeL;
  let targetUpperMinus1 = Breakpoint.rangeU(upperRange) - 1;
  media({j|(min-width: $(targetLower)px) and (max-width: $(targetUpperMinus1).9px)|j}, rules)
};

let property = (
  rule: 'a => rule,
  ~default: option('a)=?,
  ~smallPhone: option('a)=?, 
  ~phone: option('a)=?,
  ~tablet: option('a)=?,
  ~laptop: option('a)=?,
  ~desktop: option('a)=?,
  ~fullHD: option('a)=?,
  _:unit) => {
  
  let default = default 
    |. Belt.Option.map(value => [rule(value)])
    |. Belt.Option.getWithDefault([]);

  let smallPhone =
    smallPhone
    |. Belt.Option.map(value => [exactly(Breakpoint.SmallPhone, [rule(value)])])
    |. Belt.Option.getWithDefault([]);

  let phone =
    phone
    |. Belt.Option.map(value => [exactly(Breakpoint.Phone, [rule(value)])])
    |. Belt.Option.getWithDefault([]);

  let tablet =
    tablet
    |. Belt.Option.map(value => [exactly(Breakpoint.Tablet, [rule(value)])])
    |. Belt.Option.getWithDefault([]);

  let laptop =
    laptop
    |. Belt.Option.map(value => [exactly(Breakpoint.Laptop, [rule(value)])])
    |. Belt.Option.getWithDefault([]);

  let desktop =
    desktop
    |. Belt.Option.map(value => [exactly(Breakpoint.Desktop, [rule(value)])])
    |. Belt.Option.getWithDefault([]);

  let fullHD =
    fullHD
    |. Belt.Option.map(value => [exactly(Breakpoint.FullHD, [rule(value)])])
    |. Belt.Option.getWithDefault([]);
  
  default @ smallPhone @ phone @ tablet @ laptop @ desktop @ fullHD
}

let define = (
  ~default: option(list(rule))=?,
  ~smallPhone: option(list(rule))=?, 
  ~phone: option(list(rule))=?,
  ~tablet: option(list(rule))=?,
  ~laptop: option(list(rule))=?,
  ~desktop: option(list(rule))=?,
  ~fullHD: option(list(rule))=?,
  _:unit) => {
  
  let default = Belt.Option.getWithDefault(default, []);
  let smallPhone =
    smallPhone
    |. Belt.Option.map(rules => [exactly(Breakpoint.SmallPhone, rules)])
    |. Belt.Option.getWithDefault([]);
  
  let phone =
    phone
    |. Belt.Option.map(rules => [exactly(Breakpoint.Phone, rules)])
    |. Belt.Option.getWithDefault([]);
  
  let tablet =
    tablet
    |. Belt.Option.map(rules => [exactly(Breakpoint.Tablet, rules)])
    |. Belt.Option.getWithDefault([]);

  let laptop =
    laptop
    |. Belt.Option.map(rules => [exactly(Breakpoint.Laptop, rules)])
    |. Belt.Option.getWithDefault([]);

  let desktop =
    desktop
    |. Belt.Option.map(rules => [exactly(Breakpoint.Desktop, rules)])
    |. Belt.Option.getWithDefault([]);

  let fullHD =
    fullHD
    |. Belt.Option.map(rules => [exactly(Breakpoint.FullHD, rules)])
    |. Belt.Option.getWithDefault([]);

  default @ smallPhone @ phone @ tablet @ laptop @ desktop @ fullHD
};

module Classes {
  let _atLeast = breakpoint => merge([
    style([
      display(none)
    ] |> List.map(r => important(r))),

    style([
      atLeast(breakpoint, [
        display(initial)
      ] |> List.map(r => important(r)))
    ])
  ]);

  let _atMost = breakpoint => merge([
    style([
      display(none)
    ] |> List.map(r => important(r))),

    style([
      atMost(breakpoint, [
        display(initial)
      ] |> List.map(r => important(r)))
    ])
  ]);

  let _exactly = breakpoint => merge([
    style([
      display(none)
    ] |> List.map(r => important(r))),

    style([
      exactly(breakpoint, [
        display(initial)
      ] |> List.map(r => important(r)))
    ])
  ]);

  let _between = (lower, upper) => merge([
    style([
      display(none)
    ] |> List.map(r => important(r))),

    style([
      between(lower, upper, [
        display(initial)
      ] |> List.map(r => important(r)))
    ])
  ]);

  let atLeastPhone = _atLeast(Breakpoint.Phone);
  let atLeastTablet = _atLeast(Breakpoint.Tablet);
  let atLeastLaptop = _atLeast(Breakpoint.Laptop);
  let atLeastDesktop = _atLeast(Breakpoint.Desktop);
  let atLeastFullHD = _atLeast(Breakpoint.FullHD);

  let atMostSmallPhone = _atMost(Breakpoint.SmallPhone);
  let atMostPhone = _atMost(Breakpoint.Phone);
  let atMostTablet = _atMost(Breakpoint.Tablet);
  let atMostLaptop = _atMost(Breakpoint.Laptop);
  let atMostDesktop = _atMost(Breakpoint.Desktop);
  let atMostFullHD = _atMost(Breakpoint.FullHD);

  let exactlySmallPhone = _exactly(Breakpoint.SmallPhone);
  let exactlyPhone = _exactly(Breakpoint.Phone);
  let exactlyTablet = _exactly(Breakpoint.Tablet);
  let exactlyLaptop = _exactly(Breakpoint.Laptop);
  let exactlyDesktop = _exactly(Breakpoint.Desktop);
  let exactlyFullHD = _exactly(Breakpoint.FullHD);

  let betweenPhoneAndTablet = _between(Breakpoint.Phone, Breakpoint.Tablet);
  let betweenPhoneAndLaptop = _between(Breakpoint.Phone, Breakpoint.Laptop);
  let betweenPhoneAndDesktop = _between(Breakpoint.Phone, Breakpoint.Desktop);
  let betweenPhoneAndFullHD = _between(Breakpoint.Phone, Breakpoint.FullHD);
  let betweenTabletAndLaptop = _between(Breakpoint.Tablet, Breakpoint.Laptop);
  let betweenTabletAndDesktop = _between(Breakpoint.Tablet, Breakpoint.Desktop);
  let betweenTabletAndFullHD = _between(Breakpoint.Tablet, Breakpoint.FullHD);
  let betweenLaptopAndDesktop = _between(Breakpoint.Laptop, Breakpoint.Desktop);
  let betweenLaptopAndFullHD = _between(Breakpoint.Laptop, Breakpoint.FullHD);
  let betweenDesktopAndFullHD = _between(Breakpoint.Desktop, Breakpoint.FullHD);
};