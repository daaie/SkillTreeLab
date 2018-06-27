var obj={
    name:"pda",
    showNama : function(){
        console.log(this.name);
    }
}

console.log(false == 0);
console.log(false == '0');
console.log(false == undefined);
console.log(false == null);
console.log(false == '');
// == 값을 비교하는거 (형변환 규칙에 따라)
// === 타입까지 비교하는거 
// !=
// !==

//obj.showNama();
//obj["showNama"]();

for(var key in obj){
    //console.log(typeof key);
    //console.log(typeof obj[key]);

    if(typeof obj[key] === 'function'){
        obj[key]();
    }
}