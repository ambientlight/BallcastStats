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

let forgotPasswordForm = 
  <form className=Styles.form>
    <span className=Styles.welcomeTitle>{ReasonReact.string("Forgot your password?")}</span>
    <span className=merge([Styles.accesoryLabel, Styles.smallTopMargin])>{ReasonReact.string("Don't worry, please enter your email address, if there an account asssociated with it, we will send the reset password email to it shortly.")}</span>
    <TextField
      type_="email"
      label=ReasonReact.string("email")
      className=Styles.textField/>
    <Button.Blended className=Styles.button>"Send"</Button.Blended>
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
        | ForgotPassword => forgotPasswordForm
        }}
      </MaterialUi.Card>

    </div>
}