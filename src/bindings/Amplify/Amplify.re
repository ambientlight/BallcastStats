module Auth {
  module CognitoUser {
    [@bs.deriving abstract]
    type t = {
      challengeName: string
    }
  }

  [@bs.module "aws-amplify"][@bs.scope "Auth"]
  external signIn: (~username: string, ~password: string) => Js.Promise.t(CognitoUser.t) = "signIn";

  [@bs.module "aws-amplify"][@bs.scope "Auth"]
  external completeNewPassword: (~user: CognitoUser.t, ~password: string, ~requiredAttributes: Js.t({.})=?, unit) => Js.Promise.t(CognitoUser.t) = "completeNewPassword";

};