open Operators;
open MaterialUi.ThemeOptions;

let pitchGreen = Primary.make(
	~main="#226831",
	~light="#52975c",
	~dark="#003c08",
	~contrastText="#ffffff",

	~_50="#e9f7ec",
	~_100="#caead0",
	~_200="#a8ddb2",
	~_300="#84d093",
	~_400="#69c67c",
	~_500="#4fbb65",
	~_600="#46ac5b",
	~_700="#3b994f",
	~_800="#338844",
	~_900="#226831",

	~a100="#ccff90",
	~a200="#b2ff59",
	~a400="#76ff03",
	~a700="#64dd17",
	()
);

let burntGrass = Secondary.make(
	~main="#c19f25",
	~light="#f3b94d",
	~dark="#885c00",
	~contrastText="#000000",

	~_50="#f7f8e5",
	~_100="#ebecbe",
	~_200="#dedf94",
	~_300="#d3d36c",
	~_400="#cccc50",
	~_500="#c5c435",
	~_600="#c3b42e",
	~_700="#c19f25",
	~_800="#bd891a",
	~_900="#b56605",

	~a100="#ffff8d",
	~a200="#ffff00",
	~a400="#ffea00",
	~a700="#ffd600",
	()
);

let whiteText = TypeText.make(
	~primary="rgba(255, 255, 255, 1)",
	~secondary="rgba(255, 255, 255, 0.75)",
	~disabled="rgba(255, 255, 255, 0.38)",
	~hint="rgba(255, 255, 255, 0.38)",
	());

let text = whiteText;

let typography = Typography.make(
	~h1=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="300",
		~fontSize="6rem",
		~letterSpacing="-0.01562em",
		~lineHeight="1",
		()
	),
	~h2=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="300",
		~fontSize="3.75rem",
		~letterSpacing="-0.00833em",
		~lineHeight="1",
		()
	),
	~h3=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="3rem",
		~letterSpacing="0em",
		~lineHeight="1.04",
		()
	),
	~h4=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="2.125rem",
		~letterSpacing="0.00735em",
		~lineHeight="1.17",
		()
	),
	~h5=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="1.5rem",
		~letterSpacing="0em",
		~lineHeight="1.33",
		()
	),
	~h6=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="500",
		~fontSize="1.25rem",
		~letterSpacing="0.0075em",
		~lineHeight="1.6",
		()
	),
	~subtitle1=TypographyStyleOptions.make(
		~color=text|.TypeText.secondaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="1rem",
		~letterSpacing="0.00938em",
		~lineHeight="1.75",
		()
	),
	~subtitle2=TypographyStyleOptions.make(
		~color=text|.TypeText.secondaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="500",
		~fontSize="0.875rem",
		~letterSpacing="0.00714em",
		~lineHeight="1.57",
		()
	),
	~body1=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="1.2rem",
		~letterSpacing="0.00938em",
		~lineHeight="1.5",
		()
	),
	~body2=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="1rem",
		~letterSpacing="0.01071em",
		~lineHeight="1.5",
		()
	),
	~caption=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="0.75rem",
		~letterSpacing="0.03333em",
		~lineHeight="1.66",
		()
	),
	~overline=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="0.75rem",
		~letterSpacing="0.08333em",
		~lineHeight="2.66",
		()
	),
	~button=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.gobold,
		~fontWeight="300",
		~fontSize="1rem",
		~letterSpacing="0.04em",
		~lineHeight="2.66",
		()
	),

	~useNextVariants=true, 
	()
);

let theme = MaterialUi.Theme.create(
  make(
		/* TODO: synchronize breakpoints */
		~palette=PaletteOptions.make(
			~type_="dark",
			~primary=pitchGreen,
			~secondary=burntGrass,
			~text,
			()),
		~typography,
		~shape=Shape.make(~borderRadius=4.0, ()),
		()
	)
);