CC = gcc
CFLAGS += -std=c99 -pedantic -Wall -ggdb
RM = rm
SOURCES = src/main.c src/lexer/lexer.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = build/fossil

.PHONY: all directories clean

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

