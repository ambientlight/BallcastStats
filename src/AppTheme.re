open Operators;
open MaterialUi.ThemeOptions;

external styleToString: ReactDOMRe.Style.t => string = "%identity";

/* ...yeah, but it works */
let rgbAddAlpha = (rgb, alpha) => 
  rgb 
  |> Js.String.replace("rgb", "rgba")
  |> Js.String.replace(")", ", " ++ string_of_float(alpha) ++ ")");

let pitchGreen = Primary.make(
	~main="#226831",
	~light="#52975c",
  ~dark="#003c08",
  /* IMPORTANT: has to be rgb so we can add the opacity below */
	~contrastText="rgb(255, 255, 255)",

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
  
  /* IMPORTANT: has to be rgb so we can add the opacity below */
	~contrastText="rgb(0, 0, 0)",

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

let primary = pitchGreen;
let secondary = burntGrass;

let text = TypeText.make(
	/* binds to contrast colors of our palettes since layouts mostly contain palette colors on bg */
	~primary=primary|.Primary.contrastTextGet|.Belt.Option.getExn,
	~secondary=secondary|.Secondary.contrastTextGet|.Belt.Option.getExn,
	~disabled=primary|.Primary.contrastTextGet|.Belt.Option.getExn|.rgbAddAlpha(0.38),
	~hint=primary|.Primary.contrastTextGet|.Belt.Option.getExn|.rgbAddAlpha(0.65),
	());

/* TODO: probably also need to bind this to colors, but for now haven't seen where it is used */
/* 
let actionPalette = TypeAction.make(
  ~active="rgba(0, 0, 0, 0.54)",
  ~hover="rgba(0, 0, 0, 0.08)",
  ~hoverOpacity=0.08,
  ~selected="rgba(0, 0, 0, 0.14)",
  ~disabled="rgba(0, 0, 0, 0.26)",
  ~disabledBackground="rgba(0, 0, 0, 0.12)",
  ()
)
*/

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
		~fontWeight="300",
		~fontSize="3rem",
		~letterSpacing="0em",
		~lineHeight="1.04",
		()
	),
	~h4=TypographyStyleOptions.make(
		~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="300",
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
		~color=text|.TypeText.hintGet|.Belt.Option.getExn,
		~fontFamily=Fonts.jost,
		~fontWeight="400",
		~fontSize="1rem",
		~letterSpacing="0.00938em",
		~lineHeight="1.75",
		()
	),
	~subtitle2=TypographyStyleOptions.make(
		~color=text|.TypeText.hintGet|.Belt.Option.getExn,
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
		~color=text|.TypeText.hintGet|.Belt.Option.getExn,
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

let overrides = Overrides.make(
	~muiInput=InputClassKey.make(
		/* TODO: below overrides don't match in terms of transparency */
		/* ~underline=
			ReactDOMRe.Style.make(
				~borderBottom="1px solid " ++ (text|.TypeText.hintGet|.Belt.Option.getExn),
				()
			) |. ReactDOMRe.Style.unsafeAddProp("&:before", ReactDOMRe.Style.make(
				~borderBottom="1px solid " ++ (text|.TypeText.hintGet|.Belt.Option.getExn),
				()) |. styleToString)
				|. ReactDOMRe.Style.unsafeAddProp("&:hover::before", ReactDOMRe.Style.make(
				~borderBottom="1px solid " ++ (text|.TypeText.hintGet|.Belt.Option.getExn),
				()) |. styleToString)
				|. ReactDOMRe.Style.unsafeAddProp("&:after", ReactDOMRe.Style.make(
				~borderBottom="1px solid " ++ (text|.TypeText.hintGet|.Belt.Option.getExn),
				()) |. styleToString),
			() */
		),
    
  ~muiInputLabel=InputLabelClassKey.make(
    ~root=ReactDOMRe.Style.make(
      ~color=text|.TypeText.hintGet|.Belt.Option.getExn,
      ~fontFamily=typography|.Typography.subtitle1Get|.Belt.Option.getExn|.TypographyStyleOptions.fontFamilyGet|.Belt.Option.getExn,
      ~textTransform="uppercase",
      ()
    ),
    ()),

  ~muiCheckbox=CheckboxClassKey.make(
    ~root=ReactDOMRe.Style.make(
      ~color=text|.TypeText.primaryGet|.Belt.Option.getExn,
      ()
    ),
    ~colorPrimary=ReactDOMRe.Style.make(
      ~color=primary|.Primary.mainGet,
      ()
    ),
    ~colorSecondary=ReactDOMRe.Style.make(
      ~color=secondary|.Secondary.mainGet,
      ()
    ),
    ()
  ),

  ()
);

let theme = MaterialUi.Theme.create(
  make(
		/* TODO: synchronize breakpoints */
		~palette=PaletteOptions.make(
			~type_="dark",
			~primary,
			~secondary,
      ~text,
			()),
    ~typography,
    ~overrides,
    ~shape=Shape.make(~borderRadius=4.0, ()),
		()
	)
);