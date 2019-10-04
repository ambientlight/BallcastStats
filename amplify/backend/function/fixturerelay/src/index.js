/* Amplify Params - DO NOT EDIT
You can access the following resource attributes as environment variables from your Lambda function
var environment = process.env.ENV
var region = process.env.REGION
var apiCorefootballGraphQLAPIIdOutput = process.env.API_COREFOOTBALL_GRAPHQLAPIIDOUTPUT
var apiCorefootballGraphQLAPIEndpointOutput = process.env.API_COREFOOTBALL_GRAPHQLAPIENDPOINTOUTPUT

Amplify Params - DO NOT EDIT */

const path = require('path');
require('es6-promise').polyfill();
require('isomorphic-fetch');
const AWSAppSyncClient = require('aws-appsync').default;
const AWS = require('aws-sdk');
if (!('AWS_EXECUTION_ENV' in process.env && process.env.AWS_EXECUTION_ENV.startsWith('AWS_Lambda_'))){
  require('dotenv').config({ path: path.resolve(__dirname, '..', '.env')})
}

let client;
const fixturesUpdatedMutation = require('./fixtureUpdated.mutation')

const removeNonAPIFields = record => {
  delete record['__typename'];
  delete record['createdAt'];
  delete record['updatedAt'];
  delete record['status#competitionId#homeTeamId#awayTeamId'];
  delete record['homeTeamLineup'];
  delete record['awayTeamLineup'];
  return record
}

exports.handler = (event, context) => {
  if(!client){
    client = new AWSAppSyncClient({
      url: process.env.API_COREFOOTBALL_GRAPHQLAPIENDPOINTOUTPUT,
      region: process.env.REGION,
      auth: {
        type: "AWS_IAM",
        credentials: AWS.config.credentials
      },
      disableOffline: true
    });
  }

  const records = event.Records.map(record => ({
    new: AWS.DynamoDB.Converter.unmarshall(record.dynamodb.NewImage),
    old: AWS.DynamoDB.Converter.unmarshall(record.dynamodb.OldImage)
  }));

  return client.mutate({ 
    mutation: fixturesUpdatedMutation,
    variables: { fixtures: records.map(record => removeNonAPIFields(record.new)) },
    fetchPolicy: 'no-cache'
  })
    .then(
      ({data: { fixturesUpdated }}) => 
        console.log(JSON.stringify(
          fixturesUpdated.map(fx => 
            `${fx.homeTeamName} ${fx.scores.current.home} : ${fx.awayTeamName} ${fx.scores.current.away} (min: ${fx.time.min}, sec: ${fx.time.sec}, id: ${fx.id})`
          )
        )),
      console.error)
}
