var fs = require("fs");

var mysqlDB;

function connectDB(database){
    mysqlDB=database;
};

function start(req,res){
    res.redirect('/register');
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