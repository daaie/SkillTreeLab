// 라우트 처리 함수 . path : / , /start, /show, /favicon.ico.

function route(handle, path, req, res){
    if(typeof handle[path] === 'function'){
        handle[path](req,res);
    } else {
        //404 not found page 
        handle['/notFound'](req,res);
    }
}

module.exports.route = route;