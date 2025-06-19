#ifndef GP_VISUAL_H_
#define GP_VISUAL_H_

#include "./gp_game.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define CELL_WIDTH ((float) SCREEN_WIDTH / BOARD_WIDTH)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / BOARD_HEIGHT)
#define AGENT_PADDING (fminf(CELL_WIDTH, CELL_HEIGHT) / 5.0f)
#define FOOD_PADDING (AGENT_PADDING)

// Style
#define BACKGROUND_COLOR    0x353535FF
#define GRID_COLOR          0x748CABFF
#define WALL_COLOR          (GRID_COLOR)
#define AGENT_COLOR         0xDA2C38FF
#define FOOD_COLOR          0x87C38FFF

float agents_dirs[DIR_COUNT][6] = {
    // DIR_RIGHT
    {0.0, 0.0, 1.0, 0.5, 0.0, 1.0},
    // DIR_UP
    {0.0, 1.0, 0.5, 0.0, 1.0, 1.0},
    // DIR_LEFT
    {1.0, 0.0, 1.0, 1.0, 0.0, 0.5},
    // DIR_DOWN
    {0.0, 0.0, 1.0, 0.0, 0.5, 1.0},
};

int scc(int code);
void *scp(void *ptr);

#define HEX_COLOR(hex)                      \
    ((hex) >> (3 * 8)) & 0xFF,              \
    ((hex) >> (2 * 8)) & 0xFF,              \
    ((hex) >> (1 * 8)) & 0xFF,              \
    ((hex) >> (0 * 8)) & 0xFF

void render_board_grid(SDL_Renderer *renderer);
void render_agent(SDL_Renderer *renderer, Agent agent);
void render_game(SDL_Renderer *renderer, const Game *game);

void filledTriangleColor(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, float x3, float y3, Uint32 color);
void filledCircleRGBA(SDL_Renderer *renderer, int cx, int cy, int r, Uint32 color);

#endif  // GP_VISUAL_H_
