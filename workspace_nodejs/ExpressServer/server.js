var http = require('http');
var express = require('express');
var multiparty = require("connect-multiparty");

// 라우팅 설정 함수.
function setRouter(app, handle){
    for (var path in handle){
        app.get(path, handle[path]);
        app.post(path, handle[path]);
    }
}

//서버 시작함수.
function serverStart(handle){
    //서버 생성.
    var app= express();

    //미들웨어 설정.
    app.use(multiparty());

    //라우팅 설정.
    setRouter(app, handle);
    
    //서버시작.
    http.createServer(app).listen(8000);
    console.log('서버 실행중');
}

module.exports = {
    'serverStart' : serverStart
}