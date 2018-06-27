var fs = require("fs");

function start(req,res){
    fs.readFile("../html/postRequest.html",function(error,html){
        // res.writeHead(200,{'Content-Type':'text/html'});
        // res.write(html);
        // res.end();
        
        res.send(html.toString());

        // 헤더 지정해주거나. 샌드할때 투스트링으로 바꾸거나.
        // 객체라서 잘못보냄.

    });
}

function show(req,res){
    fs.readFile("../image/test.png",function(error,image){
        if(error) res.send(error+'\n');
        else res.send(image);
    });
}

function upload(req,res){
    // 미들웨어 설정해서 업로드 이미지 파싱 처리
    // <form action="/upload" method="post" enctype="multipart/form-data">
    // form-data 처리 위한 미들웨어
    // formidable, connect-multipart(), multer등
    fs.rename(req.files.image.path,"../image/test.png",function(error){
        res.send("<img src='/show' />");
    });
}

module.exports = {
    "start":start,
    "show":show,
    "upload":upload
};