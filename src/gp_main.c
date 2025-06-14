#include "./gp_game.h"
#include "./gp_visual.h"

Game game = {0};

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    
    srand(time(NULL)); // Initialize random seed
    init_game(&game);

    static_assert(AGENTS_COUNT > 0, "Not enough agents");
    print_chromo(stdout, &game.chromos[0]);

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
                case SDL_EVENT_KEY_DOWN: {
                    switch(event.key.key) {
                        case SDLK_SPACE: {
                            step_game(&game);
                        } break;

                        case SDLK_R: {
                            init_game(&game);
                        } break;
                    }
                } break;
            }
        }

        SDL_SetRenderDrawColor(renderer, HEX_COLOR(BACKGROUND_COLOR));
        scc(SDL_RenderClear(renderer));

        render_board_grid(renderer);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Simple sleep (~60FPS)
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}