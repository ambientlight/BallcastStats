open Operators;
open AWSExport;

[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

let client = AppSync.createClient(
  AppSync.createClientParams(
    ~url=config|.aws_appsync_graphqlEndpointGet,
    ~region=config|.aws_appsync_regionGet,
    ~auth=AppSync.createClientParamsAuth(
      ~type_=config|.aws_appsync_authenticationTypeGet,
      ~apiKey=config|.aws_appsync_apiKeyGet
    )
  )
);

module ListBlogsQuery = [%graphql {|
  query {
    listBlogs(limit: 10) {
      items {
        id,
        name
      }
    }
  } 
|}];

let target = client##query({
  "query": gql(. ListBlogsQuery.query),
  "variables": !!Js.Obj.empty(),
}) 
|> Rx.Observable.fromPromise
|> Rx.Observable.subscribe(~next=value => {
  Js.log(value);
});