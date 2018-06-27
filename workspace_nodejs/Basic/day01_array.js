// 배열 선언
var arrayA=[1,2,3,4,5];
var arrayB={
    "0":1,
    "1":2,
    "2":3,
    "3":4,
    "4":5
}

arrayA[arrayA.length]= arrayA.length+1;
arrayA[100]=103;
console.log(arrayA.length+"----------------length");
//배열 내부 length는 마지막 키 +1 임...ㅋㅋ뭐냐

arrayA.push(10);
console.log(arrayA.length+"----------------push");

arrayA.length=200;
console.log(arrayA.length+"----------------length");


arrayA.splice(2,102);
//추가 삭제는 push,, splice 로하는게 좋다.

// for(var ix=0; ix<5; ix++)
// {
//     console.log(arrayA[ix]);
// }

for (var ix in arrayA)
{
    console.log(arrayA[ix]);
}

// console.log(arrayA["0"]);
// console.log(arrayA[0]);
// ㄷㄷ.. 배열은 배열이아니라 객체임.ㅋㅋ...ㅋ....ㅋ......
// 배열은 빠른데..여기서는 ㅇ빠르지않음..ㅋㅋㅋ