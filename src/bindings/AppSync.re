[@bs.deriving abstract]
type createClientParamsAuth = {
  [@bs.as "type"] type_: string,
  apiKey: string
};

[@bs.deriving abstract]
type createClientParams = {
  url: string,
  region: string,
  auth: createClientParamsAuth
};

[@bs.module "aws-appsync"][@bs.new]
external createClient: createClientParams => ApolloClient.generatedApolloClient = "AWSAppSyncClient";