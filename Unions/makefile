CC=g++

CFLAGS=-c

BUILD_DIR=build
OUT_NAME=program

all: main

main: $(BUILD_DIR)/parallel.o 
	$(CC) $(BUILD_DIR)/parallel.o -o $(OUT_NAME)

$(BUILD_DIR)/parallel.o: parallel.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) parallel.cpp -o $(BUILD_DIR)/parallel.o

clean:
	rm -rf $(BUILD_DIR)/* $(OUT_NAME)
	rmdir $(BUILD_DIR)