open Operators;
open Css;

module Styles = SignInStyles;
let make = (~title, _children) => {
  ...ReasonReact.statelessComponent("SignIn"),
  render: _self => 
    <div className=Styles.root>
      <Logo.WithCaption className=Styles.logo hideCaptionOnSmall=false caption=title/>

      <MaterialUi.Card className=Styles.card>
        <form className=Styles.form>
          <span className=Styles.welcomeTitle>{ReasonReact.string("Glad to see you back!")}</span>
          <TextField
            label=ReasonReact.string("email")
            className=Styles.textField/>
          <TextField
            label=ReasonReact.string("password")
            className=Styles.textField/>
          <div className=Styles.actionPanel>
            <MaterialUi.FormControl>
              <MaterialUi.FormControlLabel 
                label=<span className=Styles.label>{ReasonReact.string("Stay signed in")}</span>
                control=<MaterialUi.Checkbox classes=[
                  MaterialUi.Checkbox.Classes.Root(Styles.checkbox),
                  MaterialUi.Checkbox.Classes.Checked(Styles.checkbox)
                ]/>
              />
            </MaterialUi.FormControl>
            <span className=([Styles.accesoryLabel, Styles.actionLabel] >|< " ")>{ReasonReact.string("Forgot password?")}</span>
          </div>
          <Button.Blended className=Styles.button>"Sign In"</Button.Blended>
          <div className=Styles.signUpContainer>
            <span className=Styles.accesoryLabel>{ReasonReact.string("Don't have an account?")}</span>
            <span className=([Styles.label, Styles.actionLabel] >|< " ")>{ReasonReact.string("Sign up")}</span>
          </div>
        </form>
      </MaterialUi.Card>
    </div>
}