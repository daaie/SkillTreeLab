// 모듈 추출.
var server = require('./server');
var requestHandler = require('./requestHandler');
var database = require('./database');

// 핸들러 생성자.
function HandleCreator(method, path, callback) {
    this.method = method;
    this.path = path;
    this.callback = callback;
};

// 핸들 객체 설정.
var handle = [
    new HandleCreator('get', '/register', requestHandler.showRegisterForm),
    new HandleCreator('get', '/', requestHandler.start),
    new HandleCreator('get', '/login', requestHandler.showLoginForm),
    new HandleCreator('post', '/register', requestHandler.register),
    new HandleCreator('post', '/login', requestHandler.login),
];

// DB 연결.
requestHandler.connectDB(database);

// 서버 시작.
server.serverStart(handle);