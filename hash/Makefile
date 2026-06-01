.PHONY: clean

main: main.o HashTable.o
	g++ main.o HashTable.o -o main

main.o: main.cpp
	g++ -c main.cpp

HashTable.o: HashTable.cpp
	g++ -c HashTable.cpp

clean:
	rm *.o main
