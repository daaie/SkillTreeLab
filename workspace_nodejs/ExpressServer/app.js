var http = require('http');
var express = require('express');

//var app = express();

//app.use(function(req,res){

    // var data={
    //     name:'pda',
    //     region:'seoul',
    //     testfunc : {
    //         x:30,
    //         y:40,
    //         z:50    
    //     },
    //     success:true
    // };

    // var jsonString=JSON.stringify(data);

    // res.writeHead(200, { 'Content-Type':'text/plain' });
    // res.write(jsonString);
    // res.write(data); // 제이슨 객체는 해석을 못함 문자열로 바꿔서 보내야함
    // res.end();
    
    // res.send(data); 
    //알아서 전달하는 express객체


    //-------------------------------------------------------------------------

    // Get 요청으로 전달받으 ㄴ정보 읽기.
    // var id = req.query['id'];
    // var pw = req.query['pw'];

    // var data = {
    //     id : id,
    //     pw : pw
    // }

    // res.send(data);

    //----------------------------------------------------------------------------
//});

// app.get('/' ,function(req,res){

//     console.log(req.header('User-Agent'));

//     var agent=req.header('User-Agent');

//     if(agent.toLowerCase().match(/firefox/)){
//         res.send('Hello FireFox');
//     } else if(agent.toLowerCase().match("chrome")){
//         res.send('Hello chrome');
//     } else if(agent.toLowerCase().match("Edge")){
//         res.send('Hello Edge');
//     } else{
//         res.send('Hello Express');
//     }

//     var id = req.query['id'];
//     var pw = req.query.pw;

//     var data = {
//         id : id,
//         pw : pw
//     };

//    // res.send(data);
// });

var app = express();

app.use(function(req,res,next){
    console.log("first");
    next();
});

app.use(function(req,res,next){
    console.log("second");
    next();
});

app.use(function(req,res,next){
    console.log("third");
    res.send("Express Server");
});

app.listen(8000);

http.createServer(app);
console.log("express 서버 실행중");