class ['a] list_iterator init = object
  val mutable current : 'a list = init

  method has_value = current <> []

  method get =
    match current with
    | hd :: tl -> hd
    | [] -> raise (Invalid_argument "no value")

  method next =
    match current with
    | hd :: tl -> current <- tl
    | [] -> raise (Invalid_argument "no value")
end

type 'a iterator = < get : 'a; has_value : bool; next : unit >

class ['a] stack init = object
  val mutable v : 'a list = init

  method pop =
    match v with
    | hd :: tl ->
      v <- tl;
      Some hd
    | [] -> None

  method push hd =
    v <- hd :: v

  method iterator : 'a iterator =
    new list_iterator v
end

class sstack init = object
  inherit [string] stack init

  method print =
    ListLabels.iter ~f:Js.log v
end

class double_stack init = object
  inherit [int] stack init as super

  method push hd =
    super#push (hd * 2)
end

class virtual shape x y = object(self)
  method virtual private contains: int -> int -> bool

  val mutable x: int = x
  method x = x

  val mutable y: int = y
  method y = y
end

class obj x =
  let () = Js.log "Creating obj" in
  object
    val field = Js.log "Initializing field"; x

  initializer
    Js.log "initializing" 
end

class type c = object method x: int end
class type d = object inherit c method y: int end

class type _rect = object
  method height : int [@@bs.set]
  method width : int [@@bs.set]
  method draw : unit -> unit
end [@bs]
type rect = _rect Js.t