
function route(handle, path, req, res){
    if(typeof handle[path] === 'function'){
        handle[path](req,res);
    } else {
        handle['/notFound'](req,res);
    }
}

module.exports.route = route;