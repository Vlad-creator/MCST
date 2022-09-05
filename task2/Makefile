CXX = gcc
CXXFLAGS = -c -Wall -O0

.PHONY: clean

all: main

main: main.o matrix.o graph.o queue.o
	 $(CXX) matrix.o graph.o queue.o main.o -o main

matrix.o: source/matrix.c
	 $(CXX) source/matrix.c $(CXXFLAGS)

graph.o: source/graph.c
	 $(CXX) source/graph.c $(CXXFLAGS)

queue.o: source/queue.c
	 $(CXX) source/queue.c $(CXXFLAGS)

main.o: main.c
	 $(CXX) main.c $(CXXFLAGS)

clean:
	rm -rf *.o *.d main

-include *.d

