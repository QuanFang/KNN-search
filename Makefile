dataLoader: main.o data.o
	g++ -o dataLoader.out main.o data.o

main.o: main.cc data.h
	g++ -c main.cc

data.o: data.cc data.h
	g++ -c data.cc

clean:
	rm *.o dataLoader
