// 모듈 추출
var http = require('http');
var express = require('express');
var bodyParser = require ('body-parser');
var mysql = require('mysql');
var fs = require('fs');

// DB설정
var client = mysql.createConnection({
    user : 'pda',
    password : 'qwer1234',
    database :'restDB',
    port : 3308
});

// DB연결
client.connect();

client.query("select * from user_info", function(error, result, fields){
    if(error){
        console.log("error : "+error);
    } else {
        console.log(result);
    }
});

client.query("insert into user_info (name, region) values (?,?)",['daae','pusan'], function(error, result, fields){
    if(error){
        console.log("error : "+error);
    } else {
        console.log(result);
    }
});


// 서버 생성
var app = express();

// 미들웨어 설정.
app.use(bodyParser.urlencoded({extended:false}));

// 라우팅 설정.

// 서버실행.
http.createServer(app).listen(8000);
console.log("서버 실행중");