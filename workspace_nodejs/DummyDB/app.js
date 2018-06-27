// 모듈추출
var http = require("http");
var express = require("express");
var body_parser = require("body-parser");
var fs = require("fs");
var db = require("./database");

// 데이터 베이스
var database = db.dummyDB;

// 서버생성.
var app = express();

// 미들웨어 설정.
app.use(body_parser.urlencoded({
    extended : false
}));

// 라우터 설정////////////////////////////////////////////////////////////////////////////

// 리디렉션.
app.get('/',function(req,res){
    res.redirect('/user');
});

// 전체 사용자 정보 조회.
app.get('/user',function(req,res){
    res.send(database.get());
});

app.get('/user/:id',function(req,res){
    // id값 추출
    var id = req.params.id;
    
    // id를 이용해서 사용자 정보 조회.
    res.send(database.get(id));
});

app.post('/user',function(req,res){
    var data =req.body;
    if(data.name && data.region)
    {
        res.send(database.insert(data));
    }else{
        throw new Error('error');
    }
});

//사용자 갱신
app.put('/user/:id',function(req,res){
    var id = req.params.id;
    var name = req.body.name;
    var region = req.body.region;

    //데이타 조회
    var item = database.get(id);
    item.name = name || item.name;
    item.region = region || item.region;

    // 요청응답.
    res.send(item);
});

app.delete('/user/:id',function(req,res){
    var id = req.params.id;
    res.send(database.remove(id));
});

// 서버실행./////////////////////////////////////////////////////////////////////////////////////
http.createServer(app).listen(8000);
console.log("서버 실행중");