// DB 변수 설정.
var mysqlDB;

// DB 연결 함수.
function connectDB(db){
    mysqlDB=db;
};

// 요청 처리 함수.
function start(req,res){
    res.redirect('/user');
}

function showTotalUser(req,res){
    mysqlDB.get(res);
}

function showUser(req,res){
    mysqlDB.get(res,req.params.id);
    //:id 로 넘어오는 거는 파람스에서 파싱함
    // http 기본 모듈임 . 이거는 바디파서가 업서도됨.
}

function insertUser(req,res){
    var data = {
        name : req.body.name,
        region : req.body.region
    }
    
    mysqlDB.insert(res,data);
    mysqlDB.insert(res,req.body);
    // html에서 post로 오는 거는  body parser가 파싱함.
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