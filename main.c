#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <SDL3/SDL.h>
#include "./style.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define CELL_WIDTH ((float) SCREEN_WIDTH / BOARD_WIDTH)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / BOARD_HEIGHT)

#define AGENTS_COUNT 5
#define AGENT_PADDING 15.0f

typedef enum {
    DIR_RIGHT = 0,
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN,
} Dir;

float agents_dirs[4][6] = {
    // DIR_RIGHT
    {0.0, 0.0, 1.0, 0.5, 0.0, 1.0},
    // DIR_UP
    {0.0, 1.0, 0.5, 0.0, 1.0, 1.0},
    // DIR_LEFT
    {1.0, 0.0, 1.0, 1.0, 0.0, 0.5},
    // DIR_DOWN
    {0.0, 0.0, 1.0, 0.0, 0.5, 1.0},
};

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

void filledTriangleColor(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, float x3, float y3, Uint32 color) {
    sdl_set_color_hex(renderer, color);

    // Find the bounding box of the triangle
    float min_y = fminf(fminf(y1, y2), y3);
    float max_y = fmaxf(fmaxf(y1, y2), y3);

    // Clip to the screen bounds
    min_y = fmaxf(min_y, 0);
    max_y = fminf(max_y, SCREEN_HEIGHT);

    for (float y = min_y; y <= max_y; y += 1.0f) {
        float intersections[3];
        int intersect_count = 0;

        // Edge1: (x1, y1) to (x2, y2)
        if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) {
            float t = (y - y1) / (y2 - y1);
            if (t >= 0 && t <= 1) {
                intersections[intersect_count++] = x1 + t * (x2 - x1);
            }
        }

        // Edge2: (x2, y2) to (x3, y3)
        if ((y2 <= y && y3 > y) || (y3 <= y && y2 > y)) {
            float t = (y - y2) / (y3 - y2);
            if (t >= 0 && t <= 1) {
                intersections[intersect_count++] = x2 + t * (x3 - x2);
            }
        }

        // Edge3: (x3, y3) to (x1, y1)
        if ((y3 <= y && y1 > y) || (y1 <= y && y3 > y)) {
            float t = (y - y3) / (y1 - y3);
            if (t >= 0 && t <= 1) {
                intersections[intersect_count++] = x3 + t * (x1 - x3);
            }
        }

        // Sort intersections
        if (intersect_count >= 2) {
            for (int i=0; i < intersect_count - 1; i++) {
                for (int j=0; j < intersect_count - i - 1; j++) {
                    if (intersections[j] > intersections[j + 1]) {
                        float temp = intersections[j];
                        intersections[j] = intersections[j + 1];
                        intersections[j + 1] = temp;
                    }
                }
            }
            // Draw line between the first two intersections
            scc(SDL_RenderLine(renderer, intersections[0], y, intersections[1], y));
        }
    }

    // Draw outline for better visibility
    SDL_FPoint points[4] = {
        {x1, y1},
        {x2, y2},
        {x3, y3},
        {x1, y1}
    };
    scc(SDL_RenderLines(renderer, points, 4));
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
    // Calculate triangle vertices
    float x1 = agents_dirs[agent.dir][0] * (CELL_WIDTH - AGENT_PADDING * 2) + agent.pos_x * CELL_WIDTH + AGENT_PADDING;
    float y1 = agents_dirs[agent.dir][1] * (CELL_HEIGHT - AGENT_PADDING * 2) + agent.pos_y * CELL_HEIGHT + AGENT_PADDING;
    float x2 = agents_dirs[agent.dir][2] * (CELL_WIDTH - AGENT_PADDING * 2) + agent.pos_x * CELL_WIDTH + AGENT_PADDING;
    float y2 = agents_dirs[agent.dir][3] * (CELL_HEIGHT - AGENT_PADDING * 2) + agent.pos_y * CELL_HEIGHT + AGENT_PADDING;
    float x3 = agents_dirs[agent.dir][4] * (CELL_WIDTH - AGENT_PADDING * 2) + agent.pos_x * CELL_WIDTH + AGENT_PADDING;
    float y3 = agents_dirs[agent.dir][5] * (CELL_HEIGHT - AGENT_PADDING * 2) + agent.pos_y * CELL_HEIGHT + AGENT_PADDING;

    // SDL_FPoint points[4] = {
    //     {x1, y1},
    //     {x2, y2},
    //     {x3, y3},
    //     {x1, y1} // Close the triangle
    // };
    // scc(SDL_RenderLines(renderer, points, 4));
    filledTriangleColor(renderer, x1, y1, x2, y2, x3, y3, AGENT_COLOR);
}

void render_all_agents(SDL_Renderer *renderer) {
    for (size_t i = 0; i < AGENTS_COUNT; ++i) {
        render_agent(renderer, agents[i]);
    }
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    
    srand(time(NULL)); // Initialize random seed
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

        sdl_set_color_hex(renderer, BACKGROUND_COLOR);
        scc(SDL_RenderClear(renderer));

        render_board_grid(renderer);
        render_all_agents(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Simple sleep (~60FPS)
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}