[@bs.module "react-code-input"][@react.component]
external make: (
  ~type_: string=?,
  ~value: string=?,
  ~fields: int=?, 
  ~className: string=?,
  ~disabled: bool=?,
  ~autoFocus: bool=?,
  ~onChange: string => unit=?,
  ~ref: ReactDOMRe.domRef=?,
  ~children: 'children=?) => React.element = "default";