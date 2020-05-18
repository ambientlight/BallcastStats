open Shortener;

type playerInfo = {
  name: string,
  number: int
};

type squad = array(playerInfo);

type colors = {
  main: int,
  secondary: int,
  text: int,
  textAccent: int
};

let compareColors = (lhs: colors, rhs: colors) => 
  lhs.main - rhs.main != 0 
    ? lhs.main - rhs.main
    : lhs.secondary - rhs.secondary != 0 
      ? lhs.secondary - rhs.secondary
      : lhs.text - rhs.text;

module Kit {
  type t = 
    | Home
    | Away
    | Alternative;
};

type gkFieldKits = {
  gk: colors,
  field: colors
};

type kits = {
  home: gkFieldKits,
  away: gkFieldKits,
  alternative: gkFieldKits
};

type t = {
  name: string,
  squad: squad,
  kits: kits
};

module View {
  type layoutMode = 
    | Home
    | Away
    | HomeCompact
    | AwayCompact;

  type _team = t;

  type t = {
    mode: layoutMode,
    name: string,
    squad: squad,
    colors: gkFieldKits,
    /***
     * home-layout formation 
     * this repsents the original formation layout (non-compacted, no-away)
     * for now pitchtr is responsible for the transformation into appropriate mode
     */
    formation: Formation.t
  };

  let create = (~team: _team, ~formation: Formation.t, ~kit: Kit.t, ~mode: layoutMode) => {
    mode,
    name: team.name,
    squad: team.squad,
    colors: switch(kit){
      | Home => team.kits.home
      | Away => team.kits.away
      | Alternative => team.kits.alternative 
    },
    formation
  };

  let homeTo = (layout: array(Formation.element), ~mode: layoutMode, ~gridBounds: ReductiveRendererCore.Grid.position) => 
    switch(mode){
    | Home => layout
    | Away => 
      layout
      |. amap(element => {...element, location: {...element.location, x: gridBounds.x -. element.location.x}})
    | HomeCompact => 
      layout
      |. amap(element => element.position == `GK ? element : {...element, location: {...element.location, x: element.location.x /. 2.}})
    | AwayCompact =>
      layout
      |. amap(element => element.position == `GK ? element : {...element, location: {...element.location, x: element.location.x /. 2.}})
      |. amap(element => {...element, location: {...element.location, x: gridBounds.x -. element.location.x}})
    };
};

let liverpool = {
  name: "Liverpool",
  squad: [|
    { name: "Allison", number: 1 },
    { name: "Robertson", number: 26 },
    { name: "Van Dijk", number: 4 },
    { name: "Matip", number: 32 },
    { name: "Arnold", number: 66 },
    { name: "Fabinho", number: 3 },
    { name: "Milner", number: 7 },
    { name: "Henderson", number: 14 },
    { name: "Origi", number: 27 },
    { name: "Mane", number: 10 },
    { name: "Shaqiri", number: 23 }
  |],
  kits: {
    home: {
      field: {
        main: int_of_string("0X971014"),
        secondary: int_of_string("0X760206"),
        text: int_of_string("0Xffffff"),
        textAccent: int_of_string("0Xffffff")
      },
      gk: { 
        main: int_of_string("0x000000"),
        secondary: int_of_string("0x251700"),
        text: int_of_string("0xc3b878"),
        textAccent: int_of_string("0xc3b878")
      }
    }, 
    away: {
      field: {
        main: int_of_string("0X971014"),
        secondary: int_of_string("0X760206"),
        text: int_of_string("0Xffffff"),
        textAccent: int_of_string("0Xffffff")
      },
      gk: { 
        main: int_of_string("0x000000"),
        secondary: int_of_string("0x251700"),
        text: int_of_string("0xc3b878"),
        textAccent: int_of_string("0xc3b878")
      }
    },
    alternative: {
      field: {
        main: int_of_string("0X971014"),
        secondary: int_of_string("0X760206"),
        text: int_of_string("0Xffffff"),
        textAccent: int_of_string("0Xffffff")
      },
      gk: { 
        main: int_of_string("0x000000"),
        secondary: int_of_string("0x251700"),
        text: int_of_string("0xc3b878"),
        textAccent: int_of_string("0xc3b878")
      }
    }
  }
};
