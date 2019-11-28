[@bs.deriving abstract]
type t = {
  aws_project_region: string,
  aws_cognito_identity_pool_id: string,
  aws_cognito_region: string,
  aws_user_pools_id: string,
  aws_user_pools_web_client_id: string,
  aws_content_delivery_bucket: string,
  aws_content_delivery_url: string,
  aws_appsync_graphqlEndpoint: string,
  aws_appsync_region: string,
  aws_appsync_authenticationType: string,
  aws_appsync_apiKey: string
};

[@bs.module "src/aws-exports"][@bs.val]
external config: t = "default";