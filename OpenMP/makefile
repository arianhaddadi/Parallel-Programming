CC=g++

CFLAGS=-c -fopenmp
LNKFLAGS=-fopenmp 
INCDIR=-I/opt/intel/ipp/include
LIBDIR=-L/opt/intel/ipp/lib
LIBFILE=-lippcore
MSVER=-msse4.1

all: main

main: main.o 
	$(CC) main.o  $(LNKFLAGS) $(LIBDIR) $(LIBFILE) -o main

main.o: main.cpp
	$(CC) $(MSVER) $(CFLAGS) $(INCDIR) main.cpp

clean:
	rm -f *o main