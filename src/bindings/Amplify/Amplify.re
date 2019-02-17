module Auth {
  module CognitoUser {
    [@bs.deriving abstract]
    type t = {
      challengeName: string
    }
  }

  [@bs.module "aws-amplify"][@bs.scope "Auth"]
  external signIn: (~username: string, ~password: string) => Js.Promise.t(CognitoUser.t) = "signIn";
};