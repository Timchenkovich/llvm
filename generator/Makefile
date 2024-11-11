app.ll:
	clang -emit-llvm -S -O2 ../app/app.c -o app.ll 

executor_generator: app.ll
	clang++ -std=c++20 -o executor_generator executor_generator.cpp

executor.cpp: executor_generator
	./executor_generator

sim.o: 
	clang -c -o sim.o ../sim/sim.c

executor: executor.cpp sim.o
		clang++ -lSDL2 $(shell llvm-config --cppflags --ldflags --libs) -o executor executor.cpp sim.o

clean:
	rm -f executor.cpp executor *.o *.ll executor_generator
