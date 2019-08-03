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
    { name: "Gomes", number: 12 },
    { name: "Fabinho", number: 3 },
    { name: "Keita", number: 8 },
    { name: "Milner", number: 7 },
    { name: "Wijnaldum", number: 5 },
    { name: "Mane", number: 10 },
    { name: "Salah", number: 11 }
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