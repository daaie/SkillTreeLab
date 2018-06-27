// http 모듈 임포트
var http = require("http");
var fs = require('fs');
var path = require('path');
var url = require("url"); 

function onRequest(request, response){
    console.log("클라이언트 요청발생"+request.url);
    console.log(url.parse(request.url));

    //라우팅
    if(request.url=='/'){
        var htmlPath = path.join(__dirname, "baseHtml.html");
            fs.readFile(htmlPath, function(error,html){
            response.writeHead(200, {'Content-Type':'text.html'});
            response.write(html);
            response.end();
        })
    } else if(request.url=='/favicon.ico'){
        // 파비콘 아이콘을 불러와서 전달하기
        fs.readFile('./favicon.png',function(error,data){
            response.writeHead(200, {'Content-Type':'image/png'});
            response.write(data);
            response.end();
        })
    }

    
}

// 서버 객체 생성
var server = http.createServer(onRequest);

// 8000 번 포트 사용
server.listen(1046);

console.log("서버 실행중");