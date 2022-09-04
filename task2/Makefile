CXX = gcc
CXXFLAGS = -c -Wall -O0

.PHONY: clean

all: main

main: main.o matrix.o graph.o
	 $(CXX) matrix.o graph.o main.o -o main

matrix.o: source/matrix.c
	 $(CXX) source/matrix.c $(CXXFLAGS)

graph.o: source/graph.c
	 $(CXX) source/graph.c $(CXXFLAGS)

main.o: main.c
	 $(CXX) main.c $(CXXFLAGS)

clean:
	rm -rf *.o *.d main

-include *.d

