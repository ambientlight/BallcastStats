
module State {
  type t = {
    value: int
  };
}

module Action {
  type t = 
    | Increment(int)
    | Decrement(int)
    | DevToolsUpdate(State.t);
};

let storeCreator = 
  ReductiveDevTools.Connectors.enhancer(
    ~options=ReductiveDevTools.Extension.enhancerOptions(
      ~name=__MODULE__, 
      ~trace=true,
      ~actionCreators={
        "decrement": (value: int) => 
          Action.Decrement(value) 
          |. ReductiveDevTools.Utilities.Serializer.serializeAction,
        "increment": (value: int) => 
          Action.Increment(value)
          |. ReductiveDevTools.Utilities.Serializer.serializeAction
      },
      ()),
    ~devToolsUpdateActionCreator=(devToolsState) => Action.DevToolsUpdate(devToolsState),
    ()
  ) 
  @@ Reductive.Store.create;

let reducer = (state: State.t, action: Action.t): State.t => {
  switch(action){
  | Increment(value) => { value: state.value + value }
  | Decrement(value) => { value: state.value - value }
  | DevToolsUpdate(state)=> state
  }
};

let store = storeCreator(
  ~reducer,
  ~preloadedState={
    value: 0
  },
  ()
);