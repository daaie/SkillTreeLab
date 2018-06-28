var fs = require("fs");

function start(req,res){
    fs.readFile("./postRequest.html",function(error,html){
        res.send(html.toString());
    });
}

function show(req,res){
    fs.readFile("./image/test.png",function(error,image){
        if(error) res.send(error+'\n');
        else res.send(image);
    });
}

function upload(req,res){
    fs.rename(req.files.image.path,"./image/test.png",function(error){
        res.send("<img src='/show' />");
    });
}

module.exports = {
    "start":start,
    "show":show,
    "upload":upload
};