open Operators;
open Rx.Observable.Operators;
open ReductiveObservable.Utils;
open Utils.Rx;

/**
 * designated for displaying user snackbar notifications 
 * captured from dispatched actions
 */

/* ASSUMED TO BE ATTACHED ONCE */
module Context {
  type action = [
    `SetWarningMessage(option(string))
  ];

  type state = {
    warningMessage: option(string)
  };

  let dispatch = ref(None);

  let make = _children => {
    ...ReasonReact.reducerComponent(__MODULE__),

    initialState: () => {
      warningMessage: None
    },

    reducer: (action: action, state: state) => 
      switch(action){
      | `SetWarningMessage(warningMessage) => { 
        ReasonReact.Update({ ...state, warningMessage }) 
      }
      },

    /**
     * we expose dispatch to epics which are external to this component
     * this also assumes there is only a single instance of this component attached at any given time
     */
    didMount: self => dispatch := Some(self.send),
    willUnmount: _self => dispatch := None,

    render: ({ state, send }) => 
      <Fragment> 
        <Snackbar key="warningMessage" 
          isOpen=(!?state.warningMessage) 
          message=?state.warningMessage 
          onExited=(_event => {
            send(`SetWarningMessage(None));
          })/>
      </Fragment>
  }
}

let cognitoErrors = (reduxObservable: Rx.Observable.t(('action, 'state))) => {
  reduxObservable
  |> optMap(fun 
    | (`SignInError(error, _), _)
    | (`CompleteNewPasswordRequestError(error), _)
    | (`SignUpError(error), _)
    | (`SignUpRequestRejected(error), _) => Some((error, Context.dispatch^)) 
    | _ => None)
  |> optMap(fun | (error, Some(dispatch)) => Some((error, dispatch)) | _ => None)
  |> tap(~next=((error: Amplify.Error.t, dispatch)) => 
    dispatch(`SetWarningMessage(Some(error|.Amplify.Error.messageGet))))
  |> empty;
};

let epic = reduxObservable => 
  Rx.Observable.merge([|
    reduxObservable |. cognitoErrors
  |]);
