CC=clang
CPP=clang++

IR:
	${CC} -emit-llvm -S -O2 app.c -o app.ll 

Exec:
	${CC} start.c sim.c app.c -lSDL2 -o app

Pass:
	${CPP} pass.cpp -fPIC -shared $(shell llvm-config --cppflags --ldflags --libs) -o libPass.so

App-with-pass: Pass
	${CC} -S -emit-llvm -v -fpass-plugin=./libPass.so app.c -o app.ll -O2

Exec-with-pass: App-with-pass
	${CC}  start.c sim.c app.ll log.c -I/usr/local/include -lSDL2 -o app-with-pass

clean: 
	rm -f app app.ll libPass.so