var router = require("./router")
var server = require("./server");
var reqHandler = require("./requestHandler")

var handle = {
    '/':reqHandler.start,
    '/start':reqHandler.start,
    '/favicon.ico':reqHandler.favicon,
    '/show':reqHandler.show,
    '/upload':reqHandler.upload,
    '/notFound':reqHandler.notFound
};

server.serverStart(router.route,handle);