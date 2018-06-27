// 객체 생성 1
var character = {
    name : "pda",
    job : "white water",
    age : 24
}
// 객체 1 키 추가
character.test ="hello";


// 객체 생성 2
var daae = new Object();
// var daae={};
daae.name=300;
daae.job="백수";
daae["test2"]="hello2";

console.log(character);
console.log(daae);

console.log(character.name);
console.log(daae.name);
console.log(daae['job']);
console.log(character['age']);
console.log(daae['age']);

console.log(character.test);
console.log(daae.test2);