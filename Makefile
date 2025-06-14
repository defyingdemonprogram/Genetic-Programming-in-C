CC = gcc
PREFIX = $(HOME)/opt/c/sdl
CFLAGS = -Wall -Wextra -ggdb -pedantic -I$(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib -lSDL3 -lm
TARGET = src/gp.c
TARGET_EXE = build/gp

gp: $(TARGET) | build
	$(CC) $(CFLAGS) -o $(TARGET_EXE) $(TARGET) $(LDFLAGS)

# Create build directory if it doesn't exist
build:
	mkdir -p build

clean:
	rm -rf $(TARGET_EXE)
