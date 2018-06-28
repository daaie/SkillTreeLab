var server = require("http");
var fs = require("fs");

function onRequest(req, res){
    if(req.url=='/'){
        res.writeHead(200, {'Content-Type':'text.html'});
        res.write("response!");
        res.end();
    } else if(req.url=='/favicon.ico'){
        fs.readFile('./image/favicon.png',function(error,data){
            res.writeHead(200, {'Content-Type':'image/png'});
            res.write(data);
            res.end();
        });
    }
}

function startServer(){
    server.createServer(onRequest).listen(8000);
}

startServer();
console.log("서버 실행중");