CC = gcc
CFLAGS += -std=c99 -pedantic -Wall -ggdb
RM = rm
COMPILER_SOURCES = src/compiler/main.c
INTERPRETER_SOURCES = src/interpreter/main.c
COMPILER_OBJECTS = $(COMPILER_SOURCES:.c=.o)
INTERPRETER_OBJECTS = $(INTERPRETER_SOURCES:.c=.o)

.PHONY: all clean

all: directories interpreter compiler

directories: build

build:
	mkdir -p build/

clean:
	$(RM) $(TARGET) $(OBJECTS)

compiler : $(COMPILER_OBJECTS)
	$(CC) $^ -o build/$@

interpreter : $(INTERPRETER_OBJECTS)
	$(CC) $^ -o build/$@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

