exports.handler = (event, context, callback) => {
  // insert code to be executed by your lambda trigger
  console.log(JSON.stringify(event));
  callback(null, event);
};
