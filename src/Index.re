open AWSAmplify

/****
 * IMPORTANT: needs to be defined as seperate typeotherwise while 
 * otherwise reasonml will generate ReactHotLoader.make as 
 * function make(param) {
 *   return Root.hot(FormationContainer$Formationembedcore.make, param);
 * }
 * 
 * rather then var make = Root.hot(FormationContainer$Formationembedcore.make);
 * when seperate componentFunc type is defined as below
 */
type componentFunc = ({. }) => React.element;

module ReactHotLoader {
  [@bs.obj]
  external makeProps: (~key: string=?, unit) => {. } = "";

  [@bs.module "react-hot-loader/root"]
  external make: ([@bs.uncurry] Js.t({..}) => React.element) => componentFunc = "hot";
};

module Hot {
  let makeProps = ReactHotLoader.makeProps;
  let make = ReactHotLoader.make(App.make);
};

[@bs.module] external euLocaleData: ReactIntl.localeData({. "locale": string}) = "react-intl/locale-data/en";
[@bs.module] external ruLocaleData: ReactIntl.localeData({. "locale": string}) = "react-intl/locale-data/ru";

ReactIntl.addLocaleData(euLocaleData);
ReactIntl.addLocaleData(ruLocaleData);

// entry setup
Amplify.amplify |. Amplify.configure(AWSExports.config);
GlobalCss.inject();
ReactDOMRe.renderToElementWithId(<Hot/>, "root");