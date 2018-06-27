// 모듈 추출
var mysql = require('mysql');

// DB설정
var client = mysql.createConnection({
    user : 'pda',
    password : 'qwer1234',
    database :'restDB',
    port : 3308
});

// DB연결
client.connect();

var mysqlDB =(function(){
    var mysqlDB={};

    // db는 비동기라 
    mysqlDB.get=function(res,id){
        console.log("db get requested");

        if(id){
            id = (typeof id==='string')?Number(id):id;
            
            var query = 'select * from user_info where id=?';

            client.query(query,id,function(error, results, fields){
                if(error){
                    console.log('쿼리 실패 select * from user_info where id='+id);
                    res.send('쿼리 실패 select * from user_info where id='+id);
                } else {
                    for(var ix in results)
                    {
                        console.log(results[ix]);
                    }
                    res.send(results);
                }
            });
        } else {
            var query = 'select * from user_info';
            client.query(query,function(error, results, fields){
                if(error){
                    console.log('쿼리 실패 select * from user_info ');
                    res.send('쿼리 실패 select * from user_info');
                } else {
                    for(var ix in results)
                    {
                        console.log(results[ix]);
                    }
                    res.send(results);
                }
            });
        }
    }

    // 데이터 추가
    mysqlDB.insert = function(res,data){
        console.log("insert requested");

        if(data.name && data.region){    

            // var query = 'insert into user_info(name, region) values set ?';
            // client.query(query,data,function(error, result, fields){

            // }); 
            // 이거는 data의 키값이랑 같아야함.

            var query = 'insert into user_info(name, region) values (?,?)';

            client.query(query,[data.name,data.region],function(error, results, fields){
                if(error){
                    console.log('insert into user_info(name, region) values ('+data.name+data.region+')');
                    res.send('insert into user_info(name, region) values ('+data.name+data.region+')');
                } else {
                    console.log("결과 : "+JSON.stringify(results));
                    res.send(results);
                }
            });
        } else {
            console.log("insert requested no data");
        }
    }

    // 데이터 update
    mysqlDB.update = function(res,data){
        console.log("update requested");

        if(data.id){
            data.id = (typeof data.id==='string')?Number(data.id):data.id;
            
            var query = 'update user_info set name = ?, region = ? where id = ?';

            client.query(query,[data.name,data.region,data.id],function(error, results, fields){
                if(error){
                    console.log('update user_info set name = ?, region = ? where id = ? : '+error);
                    res.send('update user_info set name = ?, region = ? where id = ?'+error);
                } else {
                    console.log("결과 : "+results);
                    res.redirect('/user');
                }
            });
        } else {
            console.log("update requested no data");
        }
    }
    // 데이터 delete
    mysqlDB.remove = function(res,id){
        console.log("delete requested");

        if(id){
            id = (typeof id==='string')?Number(id):id;
            
            var query = 'delete from user_info where id=?';

            client.query(query,id,function(error, results, fields){
                if(error){
                    console.log('delete from user_info where id = ? 에러: '+ error);
                    res.send('delete from user_info where id = ? 에러: '+ error);
                } else {
                    console.log("결과 : "+results);
                    res.redirect('/user');
                }
            });
        } else {
            console.log("delete requested no data");
            res.send("delete requested no data");
        }
    }

    return mysqlDB;
})();

module.exports=mysqlDB;