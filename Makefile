dataLoader: datalist.o main.o
	g++ -o dataLoader datalist.o main.o

main.o: main.cc datalist.h
	g++ -o main.cc

datalist.o: datalist.cc datalist.h
	g++ -o datalist.cc

clean:
	rm *.o dataLoader
