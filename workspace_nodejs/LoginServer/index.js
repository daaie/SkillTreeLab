var server =require ("./server");
var requestHandler = require("./requestHandler");
var database=require("./database");

// 핸들러 생성자.
function handleCreator(method, path, callback){
    this.method = method;
    this.path = path;
    this.callback = callback;
};

// 핸들 객체 설정.
var handle = [
    new handleCreator('get','/',requestHandler.start),
    new handleCreator('get','/register',requestHandler.showRegisterForm),
    new handleCreator('get','/login',requestHandler.showLoginForm),
    new handleCreator('post','/register',requestHandler.register),
    new handleCreator('post','/login',requestHandler.login),
];

//DB연결
requestHandler.connectDB(database);

//서버 시작.
server.serverStart(handle);