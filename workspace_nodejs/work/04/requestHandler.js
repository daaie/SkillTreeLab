var fs = require("fs");

function start(req, res){
        res.writeHead(200, {'Content-Type':'text/html'});
        res.write("start server");
        res.end();
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

function notFound(req,res){
    res.writeHead(404, {'Content-Type':'text.html'});
    res.write('404 Not Found.');
    res.end();
}

module.exports ={
    start:start,
    favicon:favicon,
    show:show,
    notFound:notFound
}