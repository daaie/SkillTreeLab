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
    //get
    new handleCreator('get','/',requestHandler.start),
    new handleCreator('get','/register',requestHandler.showRegisterForm),
    new handleCreator('get','/login',requestHandler.showLoginForm),
    new handleCreator('get','/board',requestHandler.showBoard),
    new handleCreator('get','/board/writing',requestHandler.showWritingForm),
    new handleCreator('get','/board/:id',requestHandler.showContent),
    // 매개변수 받는거는 맨마지막으로 와야함 /board/writing 위로 가면 에러남

    
    // post
    new handleCreator('post','/register',requestHandler.register),
    new handleCreator('post','/login',requestHandler.login),
    new handleCreator('post','/board',requestHandler.saveContent),
];

//DB연결
requestHandler.connectDB(database);

//서버 시작.
server.serverStart(handle);