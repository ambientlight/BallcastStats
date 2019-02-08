open Operators;
open Css;

type mode = SignIn | SignUp | ForgotPassword;
module Styles = AuthStyles;

let signInForm = 
  <form className=Styles.form>
    <span className=Styles.welcomeTitle>{ReasonReact.string("Glad to see you back!")}</span>
    <TextField
      type_="email"
      label=ReasonReact.string("email")
      className=Styles.textField/>
    <TextField
      type_="password"
      label=ReasonReact.string("password")
      className=Styles.textField/>
    <div className=Styles.actionPanel>
      <MaterialUi.FormControl>
        <MaterialUi.FormControlLabel 
          label=<span className=Styles.label>{ReasonReact.string("Stay signed in")}</span>
          control=<MaterialUi.Checkbox value="remember" classes=[
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
  
let signUpForm =
  <form className=Styles.form>
    <span className=Styles.welcomeTitle>{ReasonReact.string("Create your account")}</span>
    <TextField
      type_="email"
      label=ReasonReact.string("email")
      className=Styles.textField/>
    <TextField
      type_="password"
      label=ReasonReact.string("password")
      className=Styles.textField/>
    <TextField
      type_="password"
      label=ReasonReact.string("confirm password")
      className=Styles.textField/>
    <Button.Blended className=Styles.button>"Sign Up"</Button.Blended>
    <div className=Styles.signUpContainer>
      <span className=Styles.accesoryLabel>{ReasonReact.string("Already have an account?")}</span>
      <span className=([Styles.label, Styles.actionLabel] >|< " ")>{ReasonReact.string("Sign in")}</span>
    </div>
    <div className=Styles.additionalInfoContainer>
      <span className=Styles.additionalInfoLabel>{ReasonReact.string("By registering, you agree to Ballcast Stat's")}</span>
      <span className=([Styles.additionalInfoLabel, Styles.actionLabel] >|< " ")>{ReasonReact.string("Terms of Service")}</span>
      <span className=Styles.additionalInfoLabel>{ReasonReact.string("and")}</span>
      <span className=([Styles.additionalInfoLabel, Styles.actionLabel] >|< " ")>{ReasonReact.string("Privacy Policy.")}</span>
    </div>
  </form>

let make = (~mode, ~title, _children) => {
  ...ReasonReact.statelessComponent("SignIn"),
  render: _self => 
    <div className=Styles.root>
      <Logo.WithCaption 
        className=([Styles.logo, mode == SignUp ? Styles.hideLogoHackOnNarrowLayout : ""] >|< " ") 
        caption=title
        hideCaptionOnSmall=false/>
      
      <MaterialUi.Card className=Styles.card>
        {switch(mode){
        | SignIn => signInForm
        | SignUp => signUpForm
        | ForgotPassword => ReasonReact.string("")
        }}
      </MaterialUi.Card>

    </div>
}