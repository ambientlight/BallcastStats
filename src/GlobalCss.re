open Css;

let inject = () => {
  global("body", [
    margin(px(0)),
    selector("*", [boxSizing(`borderBox)])
  ])
};