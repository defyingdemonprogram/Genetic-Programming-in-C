CC = gcc
PREFIX = $(HOME)/opt/c/sdl
CFLAGS = -Wall -Wextra -ggdb -pedantic -I$(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib -lSDL3 -lm
TARGET_DIR = build
COMMON_SRC = src/gp_game.h src/gp_game.c src/gp_visual.h src/gp_visual.c


gp_trainer: $(TARGET) $(COMMON_SRC) | build
	$(CC) $(CFLAGS) -o $(TARGET_DIR)/gp_trainer src/gp_trainer.c $(LDFLAGS)


gp_simulator: src/gp_simulator.c $(COMMON_SRC) | build
	$(CC) $(CFLAGS) -o $(TARGET_DIR)/gp_simulator src/gp_simulator.c $(LDFLAGS)

# Create build directory if it doesn't exist
build:
	mkdir -p build

clean:
	rm -rf $(TARGET_DIR)
