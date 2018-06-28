var sayHello = (function(){
    var sayHello = {};

    sayHello.nodeJS = function(){

        console.log("Hello NodeJS");
    }

    return sayHello;
})();

module.exports = sayHello;