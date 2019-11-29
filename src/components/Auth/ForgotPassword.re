open Operators;
open AuthLocale;
open AuthDefs;

module Styles = AuthStyles;

[@react.component]
let make = (~state, ~dispatch: action => unit) =>
  <form className=Styles.form id="AuthForgotPasswordForm">
    <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
      <DefinedMessage message=CommonLocale.strings##forgotYourPassword/>
    </MaterialUi.Typography>
    <MaterialUi.Typography variant=`Subtitle1 className=([Styles.smallTopMargin, Styles.forgotPasswordDescription]>|<" ")>
      <DefinedMessage message=strings##forgotPasswordDescription/>
    </MaterialUi.Typography>
    <MaterialUi.TextField
      type_="email"
      label=<DefinedMessage message=CommonLocale.strings##email/>
      className=Styles.textField/>
    //TODO: forgot password
    <MaterialUi.Button variant=`Outlined className=Styles.button>
      <DefinedMessage message=CommonLocale.strings##send/>
    </MaterialUi.Button>
  </form>;