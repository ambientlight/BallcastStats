class list_iterator ('a) (init) = {
  as _;
  val mutable current: list('a) = init;
  pub has_value = current != [];
  pub get =
    switch (current) {
    | [hd, ...tl] => hd
    | [] => raise(Invalid_argument("no value"))
    };
  pub next =
    switch (current) {
    | [hd, ...tl] => current = tl
    | [] => raise(Invalid_argument("no value"))
    };
};

type iterator('a) = {
  .
  get: 'a,
  has_value: bool,
  next: unit,
};

class stack ('a) (init) = {
  as _;
  val mutable v: list('a) = init;
  pub pop =
    switch (v) {
    | [hd, ...tl] =>
      v = tl;
      Some(hd);
    | [] => None
    };
  pub push = hd => v = [hd, ...v];
  pub iterator: iterator('a) = (new list_iterator)(v);
};

class sstack (init) = {
  as self;
  inherit (class stack(string))(init);
  pub print = ListLabels.iter(~f=Js.log, v);
};

class double_stack (init) = {
  as _;
  inherit (class stack(int))(init) as super;
  pub push = hd => super#push(hd * 2);
};

class virtual shape (x, y) = {
  as self;
  pri virtual contains: (int, int) => bool;
  val mutable x: int = x;
  pub x = x;
  val mutable y: int = y;
  pub y = y;
};

class obj (x) = {
  let () = Js.log("Creating obj");
  as _;
  val field = {
    Js.log("Initializing field");
    x;
  };
};
