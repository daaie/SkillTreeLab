// 모듈 추출.
var mysql = require('mysql');

// DB 연결 정보 설정.
var client = mysql.createConnection({
    user : 'root',
    password : 'qweasd123',
    database : 'userdb'
});

// DB 연결.
client.connect(function(error){
    if (error) {
        console.log('DB 연결 오류: ' + error);
    }
});

// DB 쿼리 처리 객체 생성.
var mysqlDB = (function(){

    var mysqlDB = { };

    // 로그인.
    mysqlDB.login = function(res, data) {
        var query = 'select * from userinfo where userid=? and password=?';
        client.query(query, [data.userid, data.password], function(error, result){
            if (error) {
                console.log('쿼리 검색 실패: ' + error);
                res.send('쿼리 검색 실패: ' + error);
            } else {
                //if (result === undefined)
                if (result.length > 0 ) {
                    console.log('로그인 성공');
                    res.send('로그인 성공');
                } else {
                    console.log('로그인 실패');
                    res.send('로그인 실패');
                }
            }
        });
    };

    // 등록.
    mysqlDB.insert = function(res, data) {
        // id가 사용중인지 확인.
        var query = 'select * from userinfo where userid=?';
        client.query(query, data.userid, function(error, result){

            // 검색 결과 확인.
            if (result.length === 0) {
                query = 'insert into userinfo set ?';
                client.query(query, data, function(error){
                    if (error) {
                        console.log('사용자 추가 실패: ' + error);
                        res.send('사용자 추가 실패: ' + error);
                    } else {
                        console.log('사용자 ' + data.userid + ' 등록 완료');
                        res.send('사용자 ' + data.userid + ' 등록 완료');
                    }
                });
            } else {
                console.log('ID가 사용중입니다.');
                res.send('ID가 사용중입니다.');
            }
        });
    };

    return mysqlDB;
})();

// 모듈 내보내기.
module.exports = mysqlDB;