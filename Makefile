dataLoader: main.o data.o hashfunctions.o
	g++ -o dataLoader.out main.o data.o hashfunctions.o

main.o: main.cc data.h hashfunctions.h
	g++ -c main.cc

hashfunctions.o: data.h hashfunctions.h hashfunctions.cc
	g++ -c hashfunctions.cc

data.o: data.cc data.h
	g++ -c data.cc

clean:
	rm *.o dataLoader

test: test.o data.o
	g++ -o test.out test.o data.o

test.o: test.cc
	g++ -c test.cc
