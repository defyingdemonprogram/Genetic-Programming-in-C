CC = gcc
PREFIX = $(HOME)/opt/c/sdl
CFLAGS = -Wall -Wextra -ggdb -pedantic -I$(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib -lSDL3 -lm
TARGET = main
TARGET_EXE = build/main

all: $(TARGET)

$(TARGET): main.c | build
	$(CC) $(CFLAGS) -o $(TARGET_EXE) main.c $(LDFLAGS)

# Create build directory if it doesn't exist
build:
	mkdir -p build

clean:
	rm -rf $(TARGET_EXE)
