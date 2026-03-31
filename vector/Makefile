.PHONY: clean

main: main.o vector.o
	g++ main.o vector.o -o main

main.o: main.cpp
	g++ -c main.cpp

vector.o: vector.cpp
	g++ -c vector.cpp

clean:
	rm *.o main
