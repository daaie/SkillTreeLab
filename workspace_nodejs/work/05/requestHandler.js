var fs = require("fs");

function start(req, res){
    fs.readFile("./start.html", function(error, html){
        res.writeHead(404, {'Content-Type':'text.html'});
        res.write(html);
        res.end();
    });
}

function show(req,res){
    res.writeHead(200, {'Content-Type':'text.html'});
    res.write(JSON.stringify(req.body));
    res.end();
}

function favicon(req, res){
    fs.readFile('./image/favicon.png', function(error,image){
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
    notFound:notFound,
    show:show
}