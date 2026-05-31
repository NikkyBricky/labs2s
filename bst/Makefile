.PHONY: clean

main: main.o RBT.o
	g++ main.o RBT.o -o main

main.o: main.cpp
	g++ -c main.cpp

RBT.o: RBT.cpp
	g++ -c RBT.cpp

clean:
	rm *.o main
