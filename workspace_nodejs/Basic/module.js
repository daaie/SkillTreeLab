function addNumber(num1, num2){
    return num1+num2;
}

function substract(num1, num2){
    return num1-num2;
}

// module.exports.addNumber = addNumber;
// //module.exports.substract = substract;
// exports.substract=substract;

module.exports={
    "add" : addNumber,
    "sub" : substract,
    "pda" : {
        name:"daae"
    },
    "testArray":[0,1,2,3,4,5]
}