#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <SDL3/SDL.h>
#include "./style.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define CELL_WIDTH ((float) SCREEN_WIDTH / BOARD_WIDTH)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / BOARD_HEIGHT)

#define AGENTS_COUNT 5
#define AGENTS_PADDING 10

typedef enum {
    DIR_RIGHT = 0,
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN,
} Dir;

typedef struct {
    int pos_x, pos_y;
    Dir dir;
    int hunger;
    int health;
} Agent;

typedef enum {
    ACTION_NOP = 0,
    ACTION_STEP,
    ACTION_EAT,
    ACTION_ATTACK,
} Action;

Agent agents[AGENTS_COUNT];

int scc(int code) {
    if (code < 0) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }
    return code;
}

void *scp(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}

void sdl_set_color_hex(SDL_Renderer *renderer, Uint32 hex) {
    scc(SDL_SetRenderDrawColor(
            renderer,
            (hex >> (3 * 8)) & 0xFF,
            (hex >> (2 * 8)) & 0xFF,
            (hex >> (1 * 8)) & 0xFF,
            (hex >> (0 * 8)) & 0xFF));
}

void render_board_grid(SDL_Renderer *renderer) {
    sdl_set_color_hex(renderer, GRID_COLOR);

    for (int x = 1; x < BOARD_WIDTH; ++x) {
        scc(SDL_RenderLine(
                renderer,
                x * CELL_WIDTH,
                0,
                x * CELL_WIDTH,
                SCREEN_HEIGHT));
    }

    for (int y = 1; y < BOARD_HEIGHT; ++y) {
        scc(SDL_RenderLine(
                renderer,
                0,
                y * CELL_HEIGHT,
                SCREEN_WIDTH,
                y * CELL_HEIGHT));
    }
}

int random_int_range(int low, int high) {
    return rand() % (high - low) + low;
}

Dir random_dir(void) {
    return (Dir) random_int_range(0, 4);
}

Agent random_agent(void) {
    Agent agent = {0};
    agent.pos_x = random_int_range(0, BOARD_WIDTH);
    agent.pos_y = random_int_range(0, BOARD_HEIGHT);
    agent.dir = random_dir();
    agent.hunger = 100;
    agent.health = 100;
    return agent;
}

void init_agents(void) {
    for (size_t i = 0; i < AGENTS_COUNT; ++i) {
        agents[i] = random_agent();
    }
}
void render_agent(SDL_Renderer *renderer, Agent agent) {
    sdl_set_color_hex(renderer, AGENT_COLOR);

    SDL_FRect rect = {
        (int) floorf(agent.pos_x * CELL_WIDTH + AGENTS_PADDING),
        (int) floorf(agent.pos_y * CELL_HEIGHT + AGENTS_PADDING),
        (int) floorf(CELL_WIDTH - 2 * AGENTS_PADDING),
        (int) floorf(CELL_HEIGHT - 2 * AGENTS_PADDING),
    };

    scc(SDL_RenderFillRect(renderer, &rect));
}

void render_all_agents(SDL_Renderer *renderer) {
    for (size_t i = 0; i < AGENTS_COUNT; ++i) {
        render_agent(renderer, agents[i]);
    }
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    
    init_agents();
    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *const window = scp(SDL_CreateWindow("Hello World",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE
    ));

    SDL_Renderer *const renderer = scp(SDL_CreateRenderer(
        window, NULL
    ));

    // Main loop: wait for quit event
    SDL_Event event;
    bool running = true;
    while (running) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    running = false;
                } break;
            }
        }
        SDL_Delay(16); // Simple sleep (~60FPS)

        sdl_set_color_hex(renderer, BACKGROUND_COLOR);
        scc(SDL_RenderClear(renderer));

        render_board_grid(renderer);
        render_all_agents(renderer);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}