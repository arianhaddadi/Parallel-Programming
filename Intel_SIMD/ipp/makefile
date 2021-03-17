CC=g++

CFLAGS=-c

INCDIR=-I/opt/intel/ipp/include

LIBDIR=-L/opt/intel/ipp/lib

LIBFILE=-lippcore

BUILD_DIR=build

MSSE_VER=-msse4.1


all: main

main: main.o 
	$(CC) $(BUILD_DIR)/main.o $(LIBDIR) $(LIBFILE) -o main

main.o: main.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(MSSE_VER) $(CFLAGS) $(INCDIR) main.cpp -o $(BUILD_DIR)/main.o

clean:
	rm -rf $(BUILD_DIR)
	rm main