type playerInfo = {
  name: string,
  number: int
};

type squad = array(playerInfo);

type colors = {
  main: int,
  secondary: int,
  text: int
};

type kit = {
  home: colors,
  away: colors,
  alternative: colors
};

type t = {
  name: string,
  squad: squad,
  kit: kit
};

let liverpool = {
  name: "Liverpool",
  squad: [|
    { name: "Allison", number: 13 },
    { name: "Robertson", number: 26 },
    { name: "Van Dijk", number: 4 },
    { name: "Matip", number: 32 },
    { name: "Arnold", number: 66 },
    { name: "Fabinho", number: 3 },
    { name: "Milner", number: 7 },
    { name: "Wijnaldum", number: 5 },
    { name: "Shaqiri", number: 23 },
    { name: "Mane", number: 10 },
    { name: "Origi", number: 27 }
  |],
  kit: {
    home: {
      main: int_of_string("0X971014"),
      secondary: int_of_string("0X760206"),
      text: int_of_string("0Xffffff")
    },
    away: {
      main: int_of_string("0X971014"),
      secondary: int_of_string("0X760206"),
      text: int_of_string("0Xffffff")
    },
    alternative: {
      main: int_of_string("0X971014"),
      secondary: int_of_string("0X760206"),
      text: int_of_string("0Xffffff")
    }
  }
};

let barcelona = {
  name: "Barcelona",
  squad: [|
    { name: "Ter Stegen", number: 1 },
    { name: "Roberto", number: 20 },
    { name: "Pique", number: 3 },
    { name: "Lenglet", number: 15 },
    { name: "Alba", number: 18 },
    { name: "Vidal", number: 22 },
    { name: "Busqets", number: 5 },
    { name: "Rakitic", number: 4 },
    { name: "Suarez", number: 9 },
    { name: "Messi", number: 10 },
    { name: "Coutinho", number: 7 }
  |],
  kit: {
    home: {
      main: int_of_string("0Xd5f04d"),
      secondary: int_of_string("0Xd5f04d"),
      text: int_of_string("0X131e31")
    },
    away: {
      main: int_of_string("0Xd5f04d"),
      secondary: int_of_string("0Xd5f04d"),
      text: int_of_string("0X131e31")
    },
    alternative: {
      main: int_of_string("0Xd5f04d"),
      secondary: int_of_string("0Xd5f04d"),
      text: int_of_string("0X131e31")
    }
  }
}

