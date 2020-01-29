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

// Reductive.Store.dispatch(TestStore.store, Increment(20));
// Reductive.Store.dispatch(TestStore.store, Increment(30));
// Reductive.Store.dispatch(TestStore.store, Increment(50));
// Reductive.Store.dispatch(TestStore.store, Increment(10));
// Reductive.Store.dispatch(TestStore.store, Increment(10));

// Js.Global.setInterval(() => {
//   Reductive.Store.dispatch(TestStore.store, `Increment(10));
//   ()
// }, 1000);

// TestStore.store |. Reductive.Store.subscribe(() => Js.log(Reductive.Store.getState(TestStore.store))) |> ignore;

// Rx.range(~start=1, ~count=200, ())
// |> Rx.Operators.filter((x, _idx) => x mod 2 == 1)
// |> Rx.Operators.map((x, _idx) => x + x)
// |> Rx.Observable.subscribe(
//   ~next=x=>Js.log(x)
// )

/*
const letters = of('a', 'b', 'c');
const result = letters.pipe(
  mergeMap(x => interval(1000).pipe(map(i => x+i))),
);
result.subscribe(x => console.log(x));
*/