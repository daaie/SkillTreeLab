// 함수(객체)를 클래스 처럼 사용하는 방법
// 생성자 선언 (함수)


var pda = new name("daae", "백수");
// this 키워드는 호출한 대상임. 여기서 this == pda가 되는거임. pda.name=name이런식.
// 변수 선언을 한적도 없는데 this.name이 있는 이유..

var gg = new name("gg", 22);

console.log(pda);
console.log(gg);


// 함수는 밑에있어도 상관없다.
// 실행은 한줄한줄하는데 함수는 상관없다.
function name(name, job){
    this.name=name;
    this.job=job;
}

for (var key in pda)
{
    console.log(pda[key]);
}

console.log(pda.test);
// undefined 가 나온다.
// 보통 널체크는 언디파인드랑 널 둘다 체크한다.

pda.test=null;

console.log(pda.test);

// for (var ix=0; ix<10;++ix)
// {
//     console.log(ix);
// }