CC=g++

CFLAGS=-c

INCDIR=-I/opt/intel/ipp/include

LIBDIR=-L/opt/intel/ipp/lib

LIBFILE= -lippcore -lpthread

CST11=-std=c++11

all: main

main: main.o 
	$(CC) main.o $(LIBDIR) $(LIBFILE) $(CST11) -o main 

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCDIR) $(CST11) main.cpp

clean:
	rm -f *o main

