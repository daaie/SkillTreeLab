var http = require("http");
var express = require("express");
var bodyParser = require("body-parser");

var app = express();
var routerA = express.Router();
var routerB = express.Router();

routerA.get('/index',function(req,res){
    res.send("router A index");
});

routerA.post('/login',function(req,res){
    var name = req.body.name;
    var pw = req.body.pw;

    var data = {
        'name' : name,
        'pw' : pw
    };

    //res.write(JSON.stringify(data));
    
    var jsonString='value :'+JSON.stringify(data);
    
    res.send(jsonString);

});

routerB.get('/index',function(req,res){
    res.send("router B index");
});

//라우터 적용
app.use(bodyParser.urlencoded({
    extended:false
}));
app.use('/a',routerA);
app.use('/b',routerB);
//app.use(bodyParser());


// app.get('/a', function(req,res){
//     res.send('<a href="/b">Go to B</a>');
// });

// app.get('/b',function(req,res){
//     res.send('<a href="/a">Go to A</a>');
// });


// app.get('/page/:id', function(req,res){
//     var id = req.param("id");
//     // var id = req.param.id;
//     // var id = req.param["id"];

//     res.send(id);
// });

// app.all('*',function(req,res){
//  res.send("404 not FOund");
//});

http.createServer(app).listen(8000);
console.log("서버 실행중");