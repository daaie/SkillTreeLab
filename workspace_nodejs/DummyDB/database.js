// 클로저....
// 내부 변수는 사라지는게 맞지만 ... 자바스크립트에서는 유지가됨..
// storage, count는 유지가됨..
// 무명함수를 만들면서 호출
var dummyDB = (function(){
    var dummyDB = {};
    var storage = [];
    var count = 1;

    // 메소드.
    dummyDB.get = function(id){
        if(id){

            // 변수 가공.
            id = (typeof id === 'string') ? Number(id):id;

            // 데이터 선택.
            for(var ix in storage){
                if(storage[ix].id == id){
                    return storage[ix];
                }
            }
        } else {
            return storage;
        }

    };

    // 데이터 추가.
    dummyDB.insert=function(data){
        data.id = count++;
        storage.push(data); // 배열추가 함수
        return data;
    };

    // 데이터 제거.
    dummyDB.remove=function(id){
        if(id){

            // 변수 가공.
            id = (typeof id === 'string') ? Number(id):id;

            for(var ix in storage){
                if (storage[ix].id === id){
                    // 해당 데이터 제거.
                    storage.splice(ix,1);

                    // 제거 성공 메시지 반ㅘㄴ/
                    return true;
                }
            }

            // 제거실패.
            return false;
        }; 
    };

    return dummyDB;
})();

// 모듈 내보내기.
module.exports={
    dummyDB:dummyDB
};