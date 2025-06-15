#include "./gp_game.h"
#include "./gp_visual.h"

Game games[2] = {0};

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    
    srand(time(NULL)); // Initialize random seed

    static_assert(AGENTS_COUNT > 0, "Not enough agents");

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
    int current = 0;
    init_game(&games[current]);
    bool running = true;
    while (running) {
        while(SDL_PollEvent(&event)) {
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