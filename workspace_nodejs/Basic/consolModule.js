console.log(__dirname);
console.time('test');

var index=0;

for(var ix=0;ix<100;ix++)
{
    index+=ix*10;
}

console.log('result : '+index);

console.timeEnd("test");