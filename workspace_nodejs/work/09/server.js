var http = require ('http');
var express = require ('express');
var bodyParser = require('body-parser');

function setRouter(app, handle){
    for(var ix in handle){
        switch (handle[ix].method){
            case 'get' : app.get(handle[ix].path, handle[ix].callback);
                break;
            case 'post' : app.post(handle[ix].path, handle[ix].callback);
                break;
            case 'put' : app.put(handle[ix].path, handle[ix].callback);
                break;
            case 'delete' : app.delete(handle[ix].path, handle[ix].callback);
                break;

        }
    }
}   

function serverStart(handle){

    var app = express();

    app.use(bodyParser.urlencoded({extended : false}));

    setRouter(app, handle);

    var port = process.env.port || 8000;

    http.createServer(app).listen(port);
    console.log('서버 실행중');
}

module.exports={
    serverStart : serverStart
}