libPass.so:
	clang++ pass.cpp -std=c++17 -fPIC -shared $(shell llvm-config --cppflags --ldflags --libs) -o libPass.so

app.ll: libPass.so
	clang -S -emit-llvm -v -fpass-plugin=./libPass.so ../app/app.c -o app.ll -O2

app: app.ll
	clang  ../sim/start.c ../sim/sim.c app.ll log.c -lSDL2 -o app

trace.log: app
	./app > trace.log&
	sleep 60
	killall app

collect-results: 
	clang++ collect-results.cpp -o collect-results

all: collect-results trace.log

run: collect-results trace.log
	./collect-results

clean:
	rm -fdr *.so app app.ll collect-results