open Operators;
open Rx.Observable.Operators;
open ReductiveObservable.Utils;
open Utils.Rx;



/* ASSUMED TO BE ATTACHED ONCE */
module Context {
  type action = [
    `SetSignInError(option(string))
  ];

  type state = {
    signInError: option(string)
  };

  let dispatch = ref(None);

  let make = _children => {
    ...ReasonReact.reducerComponent(__MODULE__),

    initialState: () => {
      signInError: None
    },

    reducer: (action: action, state: state) => 
      switch(action){
      | `SetSignInError(signInError) => { 
        ReasonReact.Update({ ...state, signInError }) 
      }
      },

    /**
     * we expose dispatch to epics which are external to this component
     * this also assumes there is only a single instance of this component attached
     */
    didMount: self => dispatch := Some(self.send),
    willUnmount: _self => dispatch := None,

    render: ({ state, send }) => 
      <Fragment> 
        <Snackbar isOpen=(!?state.signInError) 
          message="Incorrect credentials" 
          onExited=(_event => {
            send(`SetSignInError(None));
          })/>
      </Fragment>
  }
}

let signInError = (reduxObservable: Rx.Observable.t(('action, 'state))) => {
  reduxObservable
  |> optMap(fun | (`SignInError(error), _) => Some((error, Context.dispatch^)) | _ => None)
  |> optMap(fun | (error, Some(dispatch)) => Some((error, dispatch)) | _ => None)
  |> tap(~next=((error: ReductiveCognito.Error.t, dispatch)) => 
    dispatch(`SetSignInError(Some(error|.ReductiveCognito.Error.messageGet))))
  |> empty;
};

let epic = reduxObservable => signInError(reduxObservable);
