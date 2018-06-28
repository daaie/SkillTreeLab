var server = require("./server");
var reqHandler = require("./requestHandler");

// 핸들러 생성자.
function handleCreator(method, path, callback){
    this.method = method;
    this.path = path;
    this.callback = callback;
};

// 핸들 객체 설정.
var handle = [
    new handleCreator('GET','/',reqHandler.start),
    new handleCreator('GET','/start',reqHandler.start),
    new handleCreator('POST','/show',reqHandler.show),
    new handleCreator('GET','/favicon.ico',reqHandler.favicon),
    new handleCreator('GET','/notFound',reqHandler.notFound),
];


server.serverStart(handle);