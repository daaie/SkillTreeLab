// 모듈 추출
var http = require("http");

function serverStart(route, handle){
    function onRequest(req,res){
        console.log(req.method);
        route(handle, req.url, req, res);
    }

    var server = http.createServer();
    server.on('request', onRequest);

    server.on('close',function(){
        console.log("서버 종료");
    });

    // setTimeout(function(){     
    //     server.close();
    //     console.log("set timeout");  
    // },1000);

    server.listen(1042);
    console.log("서버 실행중");
}

module.exports={
    'serverStart' : serverStart
}