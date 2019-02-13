module Auth {
  module CognitoUser {
    type t;
  }

  [@bs.module "aws-amplify"][@bs.scope "Auth"]
  external signIn: (~username: string, ~password: string) => Js.Promise.t(CognitoUser.t) = "signIn";
};