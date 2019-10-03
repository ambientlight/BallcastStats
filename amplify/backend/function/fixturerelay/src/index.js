/* Amplify Params - DO NOT EDIT
You can access the following resource attributes as environment variables from your Lambda function
var environment = process.env.ENV
var region = process.env.REGION
var apiCorefootballGraphQLAPIIdOutput = process.env.API_COREFOOTBALL_GRAPHQLAPIIDOUTPUT
var apiCorefootballGraphQLAPIEndpointOutput = process.env.API_COREFOOTBALL_GRAPHQLAPIENDPOINTOUTPUT

Amplify Params - DO NOT EDIT */

global.WebSocket = require('ws');
const fs = require('fs');
require('es6-promise').polyfill();
require('isomorphic-fetch');
const awsExports = require('./aws-exports');
const AUTH_TYPE = require('aws-appsync/lib/link/auth-link').AUTH_TYPE;
const AWSAppSyncClient = require('aws-appsync').default;
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

const fixturesUpdatedMutation = require('./fixtureUpdated.mutation')

exports.handler = (event, context) => {
  const records = event.Records.map(record => ({
    new: AWS.DynamoDB.Converter.unmarshall(record.dynamodb.NewImage),
    old: AWS.DynamoDB.Converter.unmarshall(record.dynamodb.OldImage)
  }));

  //console.log(process.env.ENV)
  //console.log(process.env.REGION)

  return client.mutate({ 
    mutation: fixturesUpdatedMutation,
    variables: {
      fixtures: records.map(record => {
        delete record.new['__typename']
        delete record.new['createdAt']
        delete record.new['updatedAt']
        delete record.new['status#competitionId#homeTeamId#awayTeamId']
        delete record.new['homeTeamLineup']
        delete record.new['awayTeamLineup']
        return record.new
      })
    },
    fetchPolicy: 'no-cache'
  })
    .then(
      ({data: { fixturesUpdated }}) => {
        console.log(JSON.stringify(
          fixturesUpdated.map(fx => 
            `${fx.homeTeamName} ${fx.scores.current.home} : ${fx.awayTeamName} ${fx.scores.current.away} (min: ${fx.time.min}, sec: ${fx.time.sec}, id: ${fx.id})`
          )
        ))
      },
      console.error
    )
}
