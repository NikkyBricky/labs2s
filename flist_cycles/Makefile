.PHONY: default clean
CXX = g++

CXXFLAGS = -Wall -Wextra -g -fsanitize=address
LDFLAGS = -fsanitize=address
default: main

main: main.o flist.o
	$(CXX) -g main.o flist.o -o main $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

flist.o: flist.cpp
	$(CXX) $(CXXFLAGS) -c flist.cpp

clean:
	rm *.o main
