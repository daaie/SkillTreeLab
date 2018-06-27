var fs = require("fs");

var mysqlDB;

function connectDB(database){
    mysqlDB=database;
};

function start(req,res){
    res.redirect('/register');
    // 얘는 get 으로 요청됨. form을이용하여 변경해줘야 post나 delete같은걸루가
};

function showRegisterForm(req,res){
    fs.readFile("./register.html", function(error, html){
        res.type('text/html');
        res.send(html);
    });
};

function showLoginForm(req,res){
    fs.readFile("./login.html", function(error, html){
        res.type('text/html');
        res.send(html);
    });
};

function register(req,res){
    var data = {
        userid : req.body.id,
        nickname : req.body.nickname,
        password : req.body.password,
    };

    // DB에 데이터 추가 요청.
    mysqlDB.insert(res, data);
};

function login(req,res){
    var data ={
        userid : req.body.id,
        password : req.body.password
    };

    mysqlDB.login(res,data);
};


module.exports={
    connectDB:connectDB,
    start:start,
    showRegisterForm:showRegisterForm,
    showLoginForm:showLoginForm,
    register:register,
    login:login
};