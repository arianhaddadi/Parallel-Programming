CC=g++

CFLAGS=-c

BUILD_DIR=build

MSSE_VER=-msse4.1

all: main

main: main.o 
	$(CC) $(BUILD_DIR)/main.o -o main

main.o: main.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(MSSE_VER) $(CFLAGS) main.cpp -o $(BUILD_DIR)/main.o

clean:
	rm -rf $(BUILD_DIR)
	rm main