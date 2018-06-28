var fs = require("fs");
var formidable = require("formidable");

function start(req, res){
    fs.readFile('./postRequest.html', function(error,html){
        res.writeHead(200, {'Content-Type':'text/html'});
        res.write(html);
        res.end();
    });
}

function favicon(req, res){
    fs.readFile('./image/favicon.png', function(error,image){
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
    var form = new formidable.IncomingForm();
    form.parse(req,function(error,field,files){
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

module.exports ={
    start:start,
    favicon:favicon,
    show:show,
    upload:upload,
    notFound:notFound
}