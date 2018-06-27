// 모듈 추출.
var fs = require("fs");
var formidable = require("formidable");

// paht : /, /start 처리.
function start(req, res){
    // 비동기기 때문에 파일을 읽은후 function의 error혹은 html로 데이터를 보냄.
    fs.readFile('./html/postRequest.html', function(error,html){
        // 응답. (상태코드, 데이터유형(MIME타입))
        res.writeHead(200, {'Content-Type':'text/html'});
        res.write(html);
        res.end();
    });
}

function favicon(req, res){
    // 비동기기 때문에 파일을 읽은후 function의 error혹은 html로 데이터를 보냄.
    fs.readFile('./image/favicon.png', function(error,image){
        // 응답. (상태코드, 데이터유형(MIME타입))
        res.writeHead(200,{'Content-Type':'image/png'}); 
        res.write(image);
        res.end();
    });
}

function show(req, res){
    fs.readFile('./image/test.png', function(error,image){
        res.writeHead(200,{'Content-Type':'image/png'}); 
        res.write(image);
        res.end();
    });
}

function upload(req,res){
    // formidable 모듈 초기화 
    var form = new formidable.IncomingForm();
    form.parse(req,function(error,field,files){
        console.log("파싱 완료");

        fs.rename(files.image.path, './image/test.png',function(error){
            res.writeHead(200, {'Content-Type':'text/html'});
            res.write('<img src=/show />');
            res.end();
        });
    });
}

function notFound(req,res){
    res.writeHead(404, {'Content-Type':'text.html'});
    res.write('404 Not Found.');
    res.end();
}

// 모듈 내보내기.
module.exports ={
    start:start,
    favicon:favicon,
    show:show,
    upload:upload,
    notFound:notFound
}
// 키값을 굳이 문자열로 안해도 알아서 변환함..우앙