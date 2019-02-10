let root = (state: State.t, action) => 
  switch(action){
  | `DummySetSession(sessionDummy) => {
    ...state,
    session: sessionDummy ++ "YEAH"
  }
  | _ => state
  };