[@bs.config {jsx: 3}];

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
  let make = ReactHotLoader.make(App.Root.make);
};

// entry setup
Amplify.amplify |. Amplify.configure(AWSExports.config);
GlobalCss.inject();
ReactDOMRe.renderToElementWithId(<Hot/>, "root");