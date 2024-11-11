
app:
	clang ../sim/start.c ../sim/sim.c app.c -lSDL2 -o app

app.ll:
	clang -emit-llvm -S -O2 app.c -o app.ll 

all: app.ll app

clean:
	rm -fdr app.ll app
