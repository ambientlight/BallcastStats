open AuthLocale;
open AuthDefs;

module Styles = AuthStyles;

[@react.component]
let make = (~state, ~dispatch: action => unit) =>
  <form className=Styles.form autoComplete="nope">
    <MaterialUi.Typography variant=`H4 className=Styles.welcomeTitle>
      <DefinedMessage message=strings##newPasswordRequired/>
    </MaterialUi.Typography>
    <MaterialUi.Typography variant=`Subtitle1 className=Styles.smallTopMargin>
      <DefinedMessage message=strings##newPasswordRequiredDescription/>
    </MaterialUi.Typography>
    <MaterialUi.TextField value=`String(state.password)
      className=Styles.textField
      name="password"
      type_="password"
      autoComplete="nope"
      label=<DefinedMessage message=CommonLocale.strings##password/>
      onChange=(event => dispatch(`PasswordChanged(ReactEvent.Form.target(event)##value)))
      />
    <MaterialUi.TextField value=`String(state.passwordConfirmation)
      className=Styles.textField
      type_="password"
      autoComplete="nope"
      label=<DefinedMessage message=CommonLocale.strings##confirmPassword/>
      onChange=(event => dispatch(`PasswordConfirmationChanged(ReactEvent.Form.target(event)##value)))
      />
    <MaterialUi.Button variant=`Outlined
      className=Styles.button 
      onClick=(_event => dispatch(`CompleteNewPasswordRequest()))>
      <DefinedMessage message=CommonLocale.strings##confirm/>
    </MaterialUi.Button>
  </form>;