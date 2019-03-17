module CognitoUserAttribute {
  type t;

  [@bs.deriving abstract]
  type data = {
    [@bs.as "Name"] name: string,
    [@bs.as "Value"] value: string
  };

  [@bs.send] external getValue: t => string = "getValue";
  [@bs.send] external setValue: (t, ~value: string) => t = "setValue";
  [@bs.send] external getName: t => string = "getName";
  [@bs.send] external setName: (t, ~name: string) => t = "setName";
  [@bs.send] external toString: t => string = "toString";
  [@bs.send] external toJSON: t => Js.Json.t = "toJSON";

  [@bs.module "amazon-cognito-identity-js"] [@bs.new] 
  external make: (~data: data) => t = "CognitoUserAttribute";
};

module Error {
  [@bs.deriving abstract]
  type t = {
    code: string,
    name: string,
    message: string
  };  
};

module Auth {
  module CognitoUser {
    [@bs.deriving abstract]
    type t = {
      username: string,
      [@bs.optional] challengeName: string
    }
  };

  module SignUpParams {
    [@bs.deriving abstract]
    type t = {
      username: string,
      password: string,
      [@bs.optional] attributes: Js.Dict.t(string),
      [@bs.optional] validationData: array(CognitoUserAttribute.t)
    }
  };

  module SignUpResult {
    [@bs.deriving abstract]
    type t = {
      user: CognitoUser.t,
      userConfirmed: bool,
      userSub: string
    }
  };

  [@bs.module "aws-amplify"][@bs.scope "Auth"]
  external signIn: (~username: string, ~password: string) => Js.Promise.t(CognitoUser.t) = "signIn";

  [@bs.module "aws-amplify"][@bs.scope "Auth"]
  external completeNewPassword: (~user: CognitoUser.t, ~password: string, ~requiredAttributes: Js.t({.})=?, unit) => Js.Promise.t(CognitoUser.t) = "completeNewPassword";

  [@bs.module "aws-amplify"][@bs.scope "Auth"]
  external signUp: (~params: SignUpParams.t) => Js.Promise.t(SignUpResult.t) = "signUp";
};