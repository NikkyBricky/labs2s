.PHONY: clean

main: main.o min_circle.o
	g++ main.o min_circle.o -o main

main.o: main.cpp
	g++ -c main.cpp

min_circle.o: min_circle.cpp
	g++ -c min_circle.cpp

clean:
	rm main *.o
