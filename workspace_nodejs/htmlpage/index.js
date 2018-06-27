var http = require("http");
var express =require("express");
var multiparty=require("connect-multiparty");
var fs =require("fs");

var app = express();

multiparty.uploadDir = './upload';
// multiparty.uploadDir = __dirname +'/upload'
//multiparty.uploadDir=path.join(__dirname, 'upload');

app.use(multiparty({ uploadDir : __dirname+'/upload' }));

app.get('/', function(req,res){
    fs.readFile('../html/htmlPage.html',function(error,html){
        res.send(html.toString());
    })
});

app.post ('/', function(req,res){
    console.log(req.body);
    console.log(req.files.image.path);

    fs.rename(req.files.image.path,"./upload/test.png",function(error){
        console.log("이미지 업로드 완료");
    });

    //루트경로로 리디렉션
    res.redirect('/');
});

// 앞에게 널이면 8000쓰고 널이 아니면 앞에꺼씀 
var port = process.env.port || 8000;
//process.env.port && 8000;

http.createServer(app).listen(8000);

console.log('서버 실행중');