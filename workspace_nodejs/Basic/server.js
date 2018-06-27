// http 모듈 임포트
var http = require("http");

// 서버 객체 생성
var server = http.createServer(function(request, response){
    // 클라이언트 요청 응답.
    // 헤더 및 데이터 설정
    response.writeHead(200, {'Content-Type':'text.html'});
    response.write('hello Nodejs');
    // 응답 종료.
    response.end();
});

// 8000 번 포트 사용
server.listen(8000);

console.log("서버 실행중");