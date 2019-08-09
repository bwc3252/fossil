CC = gcc
CFLAGS += -std=c99 -pedantic -Wall -ggdb
RM = rm
SOURCES = src/main.c src/lexer/lexer.c src/linked_list/list.c src/io/read_file.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = build/pineapple

.PHONY: all clean

all: directories $(TARGET)

directories: build

build:
	mkdir -p build/

clean:
	$(RM) $(TARGET) $(OBJECTS)

$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

