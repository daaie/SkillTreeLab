// 함수인자로 사용가능.
// 자료형 저장가능.
// 반환값으로 전달할 수 있다.
// 변수에 저장가능.

function hello()
{
    console.log("hello");
    return;
}

// 1. 함수 인자로 펑션 사용

var helloreturn = hello(); 
// undefined.
console.log(helloreturn);

// 2. 변수에 저장 가능
var hello2 = hello;
hello2();

var hello3 = () => {
    console.log('hello3');
}
hello3();