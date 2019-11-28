type message = {
  .
  "id": string,
  "defaultMessage": string,
};

type domTag =
  | Span
  | Div
  | H1
  | H2
  | H3
  | H4
  | H5
  | H6
  | P
  | Strong
  | B
  | I
  | Em
  | Mark
  | Small
  | Del
  | Ins
  | Sub
  | Sup;

let mapDomTagToString = tag =>
  switch (tag) {
  | Span => "span"
  | Div => "div"
  | P => "p"
  | H1 => "h1"
  | H2 => "h2"
  | H3 => "h3"
  | H4 => "h4"
  | H5 => "h5"
  | H6 => "h6"
  | Strong => "strong"
  | B => "b"
  | I => "i"
  | Em => "em"
  | Mark => "mark"
  | Small => "small"
  | Del => "del"
  | Ins => "ins"
  | Sub => "sub"
  | Sup => "sup"
  };

let mapOptDomTagToString = tag =>
  switch (tag) {
  | Some(tag) => Some(tag |> mapDomTagToString)
  | None => None
  };

// module DefinedMessage = {
//   [@bs.module "react-intl"]
//   external reactClass: ReasonReact.reactClass = "FormattedMessage";
//   let make =
//       (
//         ~message: message,
//         ~values: option(Js.t({..}))=?,
//         ~tagName: option(domTag)=?,
//         _,
//       ) =>
//     ReasonReact.wrapJsForReason(
//       ~reactClass,
//       ~props={
//         "id": message##id,
//         "defaultMessage": message##defaultMessage,
//         "values": values |> Js.Nullable.fromOption,
//         "tagName": tagName |> mapOptDomTagToString |> Js.Nullable.fromOption,
//       },
//       [||],
//     );
// };

[@bs.obj]
external _makeProps: (
  ~id: string=?,
  ~defaultMessage: string=?,
  ~values: Js.t({..})=?,
  ~tagName: string=?,
  unit
) => _ = "";

let makeProps = (
  ~message: message,
  ~values: option(Js.t({..}))=?,
  ~tagName: option(domTag)=?,
  ()
) => _makeProps(
  ~id=message##id,
  ~defaultMessage=message##defaultMessage,
  ~values?,
  ~tagName=?tagName |> mapOptDomTagToString,
  ()
);

[@bs.module "react-intl"]
external make: React.component('a) = "FormattedMessage";