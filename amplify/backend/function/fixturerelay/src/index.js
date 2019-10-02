/* Amplify Params - DO NOT EDIT
You can access the following resource attributes as environment variables from your Lambda function
var environment = process.env.ENV
var region = process.env.REGION
var apiCorefootballGraphQLAPIIdOutput = process.env.API_COREFOOTBALL_GRAPHQLAPIIDOUTPUT
var apiCorefootballGraphQLAPIEndpointOutput = process.env.API_COREFOOTBALL_GRAPHQLAPIENDPOINTOUTPUT

Amplify Params - DO NOT EDIT */global.WebSocket = require('ws');
require('es6-promise').polyfill();
require('isomorphic-fetch');
const awsExports = require('./aws-exports');
const AUTH_TYPE = require('aws-appsync/lib/link/auth-link').AUTH_TYPE;
const AWSAppSyncClient = require('aws-appsync').default;
const gql = require('graphql-tag');
const AWS = require('aws-sdk');

const url = awsExports.aws_appsync_graphqlEndpoint;
const region = awsExports.aws_project_region;
const type = awsExports.aws_appsync_authenticationType;
const client = new AWSAppSyncClient({
  url,
  region,
  auth: {
    type: "AWS_IAM",
    credentials: AWS.config.credentials
  },
  disableOffline: true
});

const query = gql(`
  query{
    fixturesByDateAndComponents(date:"2019-10-02"){
      items{
        id,
        date,
        competitionName,
        homeTeamName,
        awayTeamId
      },
      nextToken
    }
  }
`);

exports.handler = (event, context) => {
  return client.query({ query, fetchPolicy: 'network-only' })
    .then(data => console.log(data))
    .catch(console.error);
}
