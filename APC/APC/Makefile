CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
TARGET = apc
SOURCES = main.c apc.c add.c sub.c mul.c div.c

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCES) apc.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) test_apc

test: $(TARGET)
	sh tests/run_tests.sh ./$(TARGET)
