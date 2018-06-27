var testA = function(arg){
    console.log(arguments.length);
    for(var ix in arguments){
        console.log(arguments[ix]);
    }
}
// arguments.라는 함수 내부 변수임.

testA(10,20,30,40,50,60);
testA(); // 안넘겨도됨