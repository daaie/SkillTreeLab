// 모듈 추출
var mysql = require('mysql');

// DB설정
var client = mysql.createConnection({
    user : 'root',
    password : 'qwer1234',
    database :'userdb',
    port : 3308
});

// DB연결
client.connect();

var mysqlDB = (function(){
    var mysqlDB={};

    // db는 비동기라 
    mysqlDB.login=function(res,data){
        console.log("db login requested");

        if(data){
            var query = 'select * from user_info where userid=? and password=?';

            client.query(query,[data.userid, data.password],function(error, results, fields){
            //client.query(query,function(error, results, fields){
                if(error){
                    console.log('쿼리 실패 select * from user_info where userid='+data.userid + " : " + data.password);
                    res.send('쿼리 실패 select * from user_info where userid='+data.userid + " : " + data.password);
                } else {
                    console.log('results.length');
                    console.log(results);
                    if(results.length>0){
                        res.send("로그인 성공");
                    } else {
                        res.send("로그인 실패");
                    }
                }
            });
        }
    }

    // 데이터 추가
    mysqlDB.insert = function(res,data){
        console.log("insert requested");

        var query = "select * from user_info where userid=?";
        client.query(query, data.userid, function(error, results){
            if(results.length === 0){
                query = 'insert into user_info set ?';
                client.query(query, data, function(error){
                    if(error){
                        console.log("사용자 추가 실패 : "+error);
                        res.send("사용자 추가 실패: "+error);
                    } else {
                        console.log("사용자 추가 성공 : ");
                        res.send("사용자 추가 성공: ");
                    }
                });
            } else {
                console.log("아이디 사용중.");
                res.send("아이디 사용중.");
            }
        });
    }
    return mysqlDB;
})();

module.exports=mysqlDB;