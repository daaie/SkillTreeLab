var http = require("http");

function serverStart(route, handle){
    function onRequest(req,res){
        console.log(req.method);
        route(handle, req.url, req, res);
    }
    var server = http.createServer();
    server.on('request', onRequest);

    server.listen(8000);
    console.log("서버 실행중");
}

module.exports={
    'serverStart' : serverStart
}