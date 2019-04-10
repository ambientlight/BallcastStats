
[@bs.deriving abstract]
type t = {
  isDev: bool,
  shouldHotReloadEpics: bool,
  signInRequired: bool
};

[@bs.val]
external _value: t = "ENVIRONMENT";

let isDev = _value |. isDevGet;
let shouldHotReloadEpics = _value |. shouldHotReloadEpicsGet;
let signInRequired = _value |. signInRequiredGet;
