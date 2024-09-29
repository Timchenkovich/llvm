CC=clang

IR:
	${CC} -emit-llvm -S -O2 app.c -o app.ll 

Exec:
	${CC} start.c sim.c app.c -lSDL2 -o app

clean: 
	rm -f app app.ll
	