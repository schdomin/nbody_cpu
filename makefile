#ds compiler
CC = g++

#ds compiler flags
CFLAGS = -c -Wall -std=c++11

#ds default field
all: main

	$(CC) bin/main.o -o bin/nbody_cpu

#ds object files
main: src/main.cpp

	rm -rf bin
	mkdir bin
	$(CC) $(CFLAGS) src/main.cpp -o bin/main.o

#ds mark clean as independent
.PHONY: clean

#ds clean command
clean:

	rm -rf bin
