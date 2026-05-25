.PHONY: clean

main: main.o BST.o
	g++ main.o BST.o -o main

main.o: main.cpp
	g++ -c main.cpp

BST.o: BST.cpp
	g++ -c BST.cpp

clean:
	rm *.o main
