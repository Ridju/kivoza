CC = gcc
CFLAGS = -Wall -pedantic -std=c11
BUILD_DIR := build

vm: utils.o vm.o main.o
	$(CC) $(CFLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/vm.o $(BUILD_DIR)/utils.o -o $(BUILD_DIR)/vm

main.o: src/main.c 
	$(CC) $(CFLAGS) -c src/main.c -o $(BUILD_DIR)/main.o

vm.o: src/vm/vm.c
	$(CC) $(CFLAGS) -c src/vm/vm.c -o $(BUILD_DIR)/vm.o

utils.o: src/utils/utils.c
	$(CC) $(CFALGS) -c src/utils/utils.c -o $(BUILD_DIR)/utils.o

clean: 
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/vm