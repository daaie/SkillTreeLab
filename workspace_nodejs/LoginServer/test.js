var http = require("http");
var express = require("express");
var cookie_parser = require("cookie-parser");


var app = express();

app.use(cookie_parser());

app.get('/',function(req,res){
    res.clearCookie('userid');
    res.redirect('/setCookie');
});

app.get('/getCookie',function(req,res){
    // 쿠키 읽기
    res.send(req.cookies);
});

app.get('/setCookie',function(req,res){
    // 쿠키 생성
    res.cookie('userid','pda');

    res.redirect('/getCookie');
});

http.createServer(app).listen(8000);
console.log("서버 실행중");