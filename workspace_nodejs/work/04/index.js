var router = require("./route")
var server = require("./server");
var reqHandler = require("./requestHandler")

var handle = {
    '/':reqHandler.start,
    '/start':reqHandler.start,
    '/favicon.ico':reqHandler.favicon,
    '/show':reqHandler.show,
    '/notFound':reqHandler.notFound
};

server.serverStart(router.route,handle);