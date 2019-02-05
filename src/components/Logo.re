open Css;

module Core {
  let make = _children => {
    ...ReasonReact.statelessComponent("Logo"),
    render: _self => 
      <svg width="41px" height="42px" viewBox="0 0 41 42" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlnsXlink="http://www.w3.org/1999/xlink">
        <title>{ReasonReact.string("Ballcast Logo")}</title>
        <defs>
          <linearGradient x1="50%" y1="0%" x2="50%" y2="100%" id="linearGradient-1">
            <stop stopColor="#FFFFFF" offset="0%"></stop>
            <stop stopColor="#FFFFFF" stopOpacity="0.116649683" offset="100%"></stop>
          </linearGradient>
          <path d="M13.7493197,4.57224624 C13.7493197,4.57224624 12.9699651,5.85165958 13.0401593,6.53091429 C13.1103536,7.210169 13.9629584,8.31452748 14.2669253,8.81604623 C15.1101158,10.2072368 15.2519053,11.181744 14.2669253,11.9186641 C12.6210677,13.1500245 11.4671417,11.9966238 10.9799843,10.6691988 C10.4318216,9.17554407 10.4167317,8.32260257 11.1814371,6.74505892 C11.9461426,5.16751527 13.7493197,4.57224624 13.7493197,4.57224624 Z M19.8606486,10.0606535 C19.8606486,10.0606535 19.2596341,8.68840214 18.6629594,8.35629743 C18.0662847,8.02419272 16.6726132,8.08917168 16.0874448,8.05050744 C14.464214,7.94325459 13.5846173,7.50044735 13.5459291,6.27091897 C13.4812828,4.21643197 15.0879581,3.93275466 16.454743,4.29507842 C17.9926865,4.70277496 18.7000306,5.17964111 19.5536616,6.71089302 C20.4072927,8.24214494 19.8606486,10.0606535 19.8606486,10.0606535 Z M12.2282863,12.7823892 C12.2282863,12.7823892 13.7259681,12.8176234 14.2791228,12.4172061 C14.8322775,12.0167887 15.3623776,10.726232 15.6447221,10.2122296 C16.4279332,8.78640989 17.2009864,8.17636296 18.3316679,8.66092068 C20.2209862,9.47059497 19.7990748,11.0466245 18.8930697,12.1322277 C17.8736082,13.3537779 17.1424842,13.7933169 15.3939385,13.9198344 C13.6453929,14.0463519 12.2282863,12.7823892 12.2282863,12.7823892 Z" id="path-2"></path>
          <filter x="-74.1%" y="-51.2%" width="248.2%" height="243.3%" filterUnits="objectBoundingBox" id="filter-3">
            <feOffset dx="0" dy="2" in_="SourceAlpha" result="shadowOffsetOuter1"></feOffset>
            <feGaussianBlur stdDeviation="2" in_="shadowOffsetOuter1" result="shadowBlurOuter1"></feGaussianBlur>
            <feColorMatrix values="0 0 0 0 0   0 0 0 0 0   0 0 0 0 0  0 0 0 0.5 0" type_="matrix" in_="shadowBlurOuter1"></feColorMatrix>
          </filter>
          <path d="M13.563745,1.49170894 C13.563745,1.49170894 12.7843903,2.77112228 12.8545846,3.45037699 C12.9247789,4.1296317 13.7773837,5.23399018 14.0813506,5.73550894 C14.9245411,7.12669948 15.0663306,8.10120672 14.0813506,8.83812677 C12.4354929,10.0694872 11.281567,8.91608647 10.7944096,7.58866146 C10.2462469,6.09500678 10.2311569,5.24206528 10.9958624,3.66452162 C11.7605679,2.08697797 13.563745,1.49170894 13.563745,1.49170894 Z M19.6750739,6.98011616 C19.6750739,6.98011616 19.0740593,5.60786484 18.4773847,5.27576013 C17.88071,4.94365542 16.4870385,5.00863438 15.9018701,4.96997015 C14.2786393,4.86271729 13.3990426,4.41991006 13.3603544,3.19038167 C13.2957081,1.13589468 14.9023834,0.852217365 16.2691683,1.21454112 C17.8071118,1.62223766 18.5144558,2.09910381 19.3680869,3.63035573 C20.221718,5.16160764 19.6750739,6.98011616 19.6750739,6.98011616 Z M12.0427115,9.70185187 C12.0427115,9.70185187 13.5403933,9.73708615 14.093548,9.33666876 C14.6467027,8.93625138 15.1768028,7.64569473 15.4591474,7.1316923 C16.2423584,5.7058726 17.0154117,5.09582567 18.1460932,5.58038338 C20.0354115,6.39005768 19.6135001,7.96608725 18.707495,9.05169041 C17.6880335,10.2732406 16.9569094,10.7127796 15.2083638,10.8392971 C13.4598182,10.9658146 12.0427115,9.70185187 12.0427115,9.70185187 Z" id="path-4"></path>
          <filter x="-68.8%" y="-56.3%" width="237.6%" height="233.1%" filterUnits="objectBoundingBox" id="filter-5">
            <feOffset dx="0" dy="1" in_="SourceAlpha" result="shadowOffsetOuter1"></feOffset>
            <feGaussianBlur stdDeviation="2" in_="shadowOffsetOuter1" result="shadowBlurOuter1"></feGaussianBlur>
            <feColorMatrix values="0 0 0 0 0   0 0 0 0 0   0 0 0 0 0  0 0 0 0.5 0" type_="matrix" in_="shadowBlurOuter1"></feColorMatrix>
          </filter>
        </defs>
        <g id="Page-1" stroke="none" strokeWidth="1" fill="none" fillRule="evenodd">
          <g id="Desktop-HD-Copy" transform="translate(-66.000000, -30.000000)">
            <g id="Logo" transform="translate(66.000000, 29.000000)">
              <path d="M12.118632,16.4399777 C12.118632,16.4399777 16.4234204,16.9814927 18.0397525,16.4399777 C19.6560845,15.8984627 20.8378941,13.5187888 20.8378941,13.5187888 C20.8378941,13.5187888 21.9508124,22.6451363 18.0397525,30.8827322 C14.1286925,39.1203281 0.222229844,42.6273108 0.222229844,42.6273108 C0.222229844,42.6273108 13.152226,35.4447081 14.8147234,28.6179628 C16.4772209,21.7912175 12.118632,16.4399777 12.118632,16.4399777 Z" id="Path-2" fill="url(#linearGradient-1)"></path>
              <g id="Combined-Shape" transform="translate(15.309460, 9.044995) rotate(15.000000) translate(-15.309460, -9.044995) ">
                <use fill="black" fillOpacity="1" filter="url(#filter-3)" xlinkHref="#path-2"></use>
                <use fill="#FFFFFF" fillRule="evenodd" xlinkHref="#path-2"></use>
              </g>
              <g id="Group" transform="translate(32.213390, 21.296539) rotate(175.000000) translate(-32.213390, -21.296539) translate(21.713390, 1.296539)">
                <path d="M12.2397981,12.498747 C12.2397981,12.498747 14.527397,13.2951867 16.4945765,12.793385 C18.4617561,12.2915833 20.1779655,10.4738246 20.1779655,10.4738246 C20.1779655,10.4738246 21.312797,19.5603794 17.4017371,27.7979753 C13.4906772,36.0355712 0.649330085,39.4896707 0.649330085,39.4896707 C0.649330085,39.4896707 12.5142107,32.3599512 14.1767081,25.5332059 C15.8392056,18.7064606 12.2397981,12.498747 12.2397981,12.498747 Z" id="Path-2" fill="url(#linearGradient-1)" transform="translate(10.490163, 24.981748) rotate(1.000000) translate(-10.490163, -24.981748) "></path>
                <g id="Combined-Shape" transform="translate(15.123885, 5.964458) rotate(1.000000) translate(-15.123885, -5.964458) ">
                  <use fill="black" fillOpacity="1" filter="url(#filter-5)" xlinkHref="#path-4"></use>
                  <use fill="#FFFFFF" fillRule="evenodd" xlinkHref="#path-4"></use>
                </g>
              </g>
            </g>
          </g>
        </g>
      </svg>
  };
};

let make = Core.make;

module WithCaption {
  let captionStyle = style([
    marginLeft(px(16)),

    color(white),
    fontSize(em(1.2)),
    fontFamily(Fonts.jost),
    fontWeight(`normal),
    letterSpacing(pxFloat(0.5))
  ]);

  let make = (~caption, _children) => {
    ...ReasonReact.statelessComponent("Logo.WithCaption"),
    render: _self => 
      <div className=style([display(`flex), alignItems(center)])>
        <Core/>
        <span className=merge([captionStyle, Media.Classes.atLeastTablet])>
          {ReasonReact.string(caption)}
        </span>
      </div>
  }
};