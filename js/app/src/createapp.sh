head -n1 ../dist/app.js > myapp.js
echo "import {NativeElement} from './native.so';" >> myapp.js
tail -n+2 ../dist/app.js >> myapp.js
#qjs ./myapp.js
qjsc -c -M "./native.so",NativeElement -m myapp.js -o /home/key2/Downloads/cimtest/glue/bytecode.h
