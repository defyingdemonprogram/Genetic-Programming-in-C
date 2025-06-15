#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL3/SDL.h>

#include "./gp_game.c"
#include "./gp_visual.c"

Game games[2] = {0};

void usage(FILE *stream) {
    fprintf(stream, "./gp_simulator [input.bin]\n");
}

const char *shift(int *argc, char ***argv) {
    assert(*argc > 0);
    const char *result = **argv;
    *argc -= 1;
    *argv += 1;
    return result;
}

int main(int argc, char *argv[]) {
    int current = 0;
    srand(time(0));

    shift(&argc, &argv);        // skip program name

    if (argc == 0) {
        usage(stderr);
        fprintf(stderr, "WARNING: No input file is provided. Generating a new random state...\n");
        init_game(&games[current]);
    } else {
        const char *input_filepath = shift(&argc, &argv);
        printf("Loading the state from %s...\n", input_filepath);
        load_game(input_filepath, &games[current]);
    }

    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *const window = scp(SDL_CreateWindow("Hunger Game - Genetic Programming",
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
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    running = false;
                } break;
                case SDL_EVENT_KEY_DOWN: {
                    switch(event.key.key) {
                        case SDLK_SPACE: {
                            step_game(&games[current]);
                        } break;

                        case SDLK_R: {
                            init_game(&games[current]);
                        } break;
                        case SDLK_N: {
                            int next = 1 - current;
                            make_next_generation(&games[current], &games[next]);
                            current = next;
                        } break;
                    }
                } break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    Coord pos;
                    pos.x = (int) floorf(event.button.x / CELL_WIDTH);
                    pos.y = (int) floorf(event.button.y / CELL_HEIGHT);
                    Agent *agent = agent_at(&games[current], pos);

                    if (agent) {
                        print_agent(stdout, agent);
                    }
                } break;
            }
        }

        SDL_SetRenderDrawColor(renderer, HEX_COLOR(BACKGROUND_COLOR));
        scc(SDL_RenderClear(renderer));

        // render_board_grid(renderer);
        render_game(renderer, &games[current]);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Simple sleep (~60FPS)
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}