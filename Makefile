CC=clang

app:
	${CC} start.c sim.c app.c -lSDL2 -o app

clean: 
	rm -f app
	