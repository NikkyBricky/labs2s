main: main.o Stack.o StackImplementation.o vector.o list.o
	g++ main.o Stack.o StackImplementation.o vector.o list.o -o main

main.o: main.cpp
	g++ -c main.cpp

Stack.o: ../stack/Stack.cpp
	g++ -c ../stack/Stack.cpp

StackImplementation.o: ../stack/StackImplementation.cpp
	g++ -c ../stack/StackImplementation.cpp

vector.o: ../stack/vector.cpp
	g++ -c ../stack/vector.cpp
list.o: ../stack/list.cpp
	g++ -c ../stack/list.cpp
