var server = require("./server");
var requestHandler = require("./requestHandler");

var handle = {
    '/' : requestHandler.start,
    '/start':requestHandler.start,
    '/show':requestHandler.show,
    '/upload':requestHandler.upload
};

server.serverStart(handle);
