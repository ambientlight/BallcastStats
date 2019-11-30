//FIXME: bs-react-intl is not on jsx3 yet
[@bs.module "react-intl"][@react.component]
  external make: (
    ~locale: string=?,
    ~formats: option(Js.t({..}))=?, /* TODO */
    ~messages: Js.Dict.t(string)=?,
    ~defaultLocale: option(string)=?,
    ~defaultFormats: option(Js.t({..}))=?, /* TODO */
    //~textComponent: option(textComponent)=?,
    ~initialNow: option(int)=?,
    //~onError: option(errorHandler)=?,
    ~children: 'children
  ) => React.element = "IntlProvider";