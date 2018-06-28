var mysqlDB;

function connectDB(db){
    mysqlDB=db;
};

function start(req,res){
    res.redirect('/user');
}

function showTotalUser(req,res){
    mysqlDB.get(res);
}

function showUser(req,res){
    mysqlDB.get(res,req.params.id);
}

function insertUser(req,res){
    var data = {
        name : req.body.name,
        region : req.body.region
    };
    console.log(req.body);
    mysqlDB.insert(res,data);
}

function updateUser(req,res){
    var data = {
        name : req.body.name,
        region : req.body.region,
        id : req.params.id
    };
    mysqlDB.update(res,data);
}

function removeUser(req,res){
    mysqlDB.remove(res, req.params.id);
}

module.exports={
    connectDB:connectDB,
    start:start,
    showTotalUser:showTotalUser,
    showUser:showUser,
    insertUser:insertUser,
    updateUser:updateUser,
    removeUser:removeUser
}