function start(){
    console.log('start called');
}

function show(){

    console.log('show called');
}

var handler={
    '/' : start,
    '/start' : start,
    '/show':show
}

//라우팅
function router(path, handle){
    handle[path]();
}

router('/', handler);