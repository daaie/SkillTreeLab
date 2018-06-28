// 모듈 추가.
var fs = require('fs');

// DB 변수.
var mysqlDB;

// DB 연결 함수.
function connectDB(database) {
    mysqlDB = database;
};

function start(req, res) {
    res.redirect('/register');
}

// 사용자 정보 등록하는 웹폼(webform)을 보여주는 함수.
function showRegisterForm(req, res) { 
    fs.readFile('./register.html', function(error, html){
        res.type('text/html');
        res.send(html);
    });
};

function showLoginForm(req, res) {
    fs.readFile('./login.html', function(error, html){
        res.type('text/html');
        res.send(html);
    });
};

// 사용자 정보 등록.
function register(req, res) {

    // 추가할 데이터 설정.
    var data = {
        userid : req.body.id,
        nickname : req.body.nickname,
        password : req.body.password,
    };

    // DB에 데이터 추가 요청.
    mysqlDB.insert(res, data);
}

function login(req, res) {

    // 로그인 확인할 정보 설정.
    var data = {
        userid : req.body.id,
        password : req.body.password
    };

    // 로그인 요청.
    mysqlDB.login(res, data);
}

// 모듈 내보내기.
module.exports = {
    connectDB : connectDB,
    start : start,
    showRegisterForm : showRegisterForm,
    showLoginForm : showLoginForm,
    register : register,
    login : login
};