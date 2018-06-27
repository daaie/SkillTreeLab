// 모듈 추출.
var server = require("./server");
var requestHandler = require('./requestHandler');
var database = require('./database');

// 핸들러 생성자
function handleCreator(method, path, callback){
    this.method=method,
    this.path=path,
    this.callback=callback
};

// 핸들 객체 설정 - 배열.
var handle = [
    new handleCreator('get','/',requestHandler.start),
    new handleCreator('get','/user',requestHandler.showTotalUser),
    new handleCreator('get','/user/:id',requestHandler.showUser),
    new handleCreator('post','/user',requestHandler.insertUser),
    new handleCreator('put','/user/:id',requestHandler.updateUser),
    new handleCreator('delete','/user/:id',requestHandler.removeUser)
];

// 데이터 베이스 연결.
requestHandler.connectDB(database);

// 서버 시작.
server.serverStart(handle);