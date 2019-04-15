exports.handler = function (event, context) { //eslint-disable-line
  console.log(event);
  context.done(null, event.arguments.msgP1 + event.arguments.msgP2); // SUCCESS with message
};
