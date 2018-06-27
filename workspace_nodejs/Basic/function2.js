function test(){
    // var pda = {
    //     name: 'pda',
    //     age : 24
    // };
    // return pda;
    // 리턴을 생략하면 return만하는거랑 동일.
    // 리턴만 하면 undefined가 리턴된다.

    return {
        name: 'pda',
        age : 24
    };

    // return 
    // {
    //     name: 'pda',
    //     age : 24
    // }; 
    // 에러남. -> return ; 로 인식
}

var ret = test();

console.log(ret);