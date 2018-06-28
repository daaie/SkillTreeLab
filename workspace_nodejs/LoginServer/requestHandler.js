var fs = require("fs");
var ejs = require('ejs');
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

function showBoard(req,res){
// 게시판 글 목록 보여주기.
    fs.readFile('./board.html', 'utf-8', function(error,html){
        if(error){
            res.send("error: "+error);
        }else{
            mysqlDB.showBoard(req,res,ejs,html);
        }
    });

};

function showWritingForm(req,res){
    // 게시판 글 목록 보여주기.
    fs.readFile("./writeForm.html", function(error, html){
        if(error){
            console.log("error : "+error)
            res.send("error : "+error);
        } else {
            res.type("text/html");
            res.send(html);
        }
    });
};
  
function saveContent(req,res){
    var content ={
        userid:req.cookies['userid'],
        title :req.body.title,
        content:req.body.content
    };

    mysqlDB.saveContent(req,res,content);
};

function showContent(req,res){
    //ejs를 쓸거라서 utf-8설정함 다른애들은 설정해줘도되는데
    // 기본이 유티에프팔이라서 안해줌 근데 ejs는 해줘야함.
    fs.readFile('./showContent.html','utf-8',function(error,html){
        mysqlDB.showContent(req,res,ejs,html);
    });
};

module.exports={
    connectDB:connectDB,
    start:start,
    showRegisterForm:showRegisterForm,
    showLoginForm:showLoginForm,
    register:register,
    login:login,
    showBoard:showBoard,
    showWritingForm:showWritingForm,
    saveContent:saveContent,
    showContent:showContent
};