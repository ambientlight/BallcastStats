/* Amplify Params - DO NOT EDIT
You can access the following resource attributes as environment variables from your Lambda function
var environment = process.env.ENV
var region = process.env.REGION
var analyticsBallcaststatsKinesisStreamArn = process.env.ANALYTICS_BALLCASTSTATS_KINESISSTREAMARN

Amplify Params - DO NOT EDIT */exports.handler = function (event, context) {
  context.done(null, event.arguments.msg);
};