
function exitListener(){
    console.log("byebye");
}
process.on('exit',exitListener);
process.removeListenet('exit',exitListener);
process.once('exit',exitListener);

process.on('uncaughtException',function(error){
    console.log('error : ',error);
});

var count =0;

function test(){
    count ++;
    if(count>3){
        return;
    }

    setTimeout(test,2000); // ms
    //error.error.error();
    process.emit('uncaughtException');
};

// on, emit (호출)

setTimeout(test,2000);
