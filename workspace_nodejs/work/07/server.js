var http = require('http');
var express = require('express');
var multiparty = require("connect-multiparty");

function setRouter(app, handle){
    for (var path in handle){
        app.get(path, handle[path]);
        app.post(path, handle[path]);
    }
}

function serverStart(handle){
    var app= express();
    app.use(multiparty());
    setRouter(app, handle);
    
    http.createServer(app).listen(8000);
    console.log('서버 실행중');
}

module.exports = {
    'serverStart' : serverStart
}