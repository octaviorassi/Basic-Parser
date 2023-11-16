CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
BUILD_DIR = build

all: parser testgenerator tests

parser: $(BUILD_DIR)/main.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/GQUEUES.o $(BUILD_DIR)/GLIST.o $(BUILD_DIR)/trie.o
	$(CC) $(CFLAGS) -o parser $(BUILD_DIR)/main.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/GQUEUES.o $(BUILD_DIR)/GLIST.o $(BUILD_DIR)/trie.o

testgenerator: $(BUILD_DIR)/testgenerator.o
	$(CC) $(CFLAGS) -o testgenerator $(BUILD_DIR)/testgenerator.o

tests: $(BUILD_DIR)/tests.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/GQUEUES.o $(BUILD_DIR)/GLIST.o $(BUILD_DIR)/trie.o
	$(CC) $(CFLAGS) -o tests $(BUILD_DIR)/tests.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/GQUEUES.o $(BUILD_DIR)/GLIST.o $(BUILD_DIR)/trie.o

$(BUILD_DIR)/main.o: main.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c main.c -o $@

$(BUILD_DIR)/parser.o: parser.c parser.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c parser.c -o $@

$(BUILD_DIR)/GQUEUES.o: GQUEUES.c GQUEUES.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c GQUEUES.c -o $@

$(BUILD_DIR)/GLIST.o: GLIST.c GLIST.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c GLIST.c -o $@

$(BUILD_DIR)/trie.o: trie.c trie.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c trie.c -o $@

$(BUILD_DIR)/testgenerator.o: testgenerator.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c testgenerator.c -o $@

$(BUILD_DIR)/tests.o: tests.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c tests.c -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o parser testgenerator tests
