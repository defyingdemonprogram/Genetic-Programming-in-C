#include "./gp_visual.h"

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

void render_board_grid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, HEX_COLOR(GRID_COLOR));

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

void render_agent(SDL_Renderer *renderer, Agent agent) {
    // Calculate triangle vertices
    float x1 = agents_dirs[agent.dir][0] * (CELL_WIDTH - AGENT_PADDING * 2) + agent.pos.x * CELL_WIDTH + AGENT_PADDING;
    float y1 = agents_dirs[agent.dir][1] * (CELL_HEIGHT - AGENT_PADDING * 2) + agent.pos.y * CELL_HEIGHT + AGENT_PADDING;
    float x2 = agents_dirs[agent.dir][2] * (CELL_WIDTH - AGENT_PADDING * 2) + agent.pos.x * CELL_WIDTH + AGENT_PADDING;
    float y2 = agents_dirs[agent.dir][3] * (CELL_HEIGHT - AGENT_PADDING * 2) + agent.pos.y * CELL_HEIGHT + AGENT_PADDING;
    float x3 = agents_dirs[agent.dir][4] * (CELL_WIDTH - AGENT_PADDING * 2) + agent.pos.x * CELL_WIDTH + AGENT_PADDING;
    float y3 = agents_dirs[agent.dir][5] * (CELL_HEIGHT - AGENT_PADDING * 2) + agent.pos.y * CELL_HEIGHT + AGENT_PADDING;

    filledTriangleColor(renderer, x1, y1, x2, y2, x3, y3, AGENT_COLOR);
}

void render_game(SDL_Renderer *renderer, const Game *game) {
    for (size_t i = 0; i < AGENTS_COUNT; ++i) {
        render_agent(renderer, game->agents[i]);
    }

    // TODO: foods are not rendered
    for (size_t i = 0; i < FOODS_COUNT; ++i) {
        if (!game->foods[i].eaten) {
            filledCircleRGBA(
                renderer,
                (int) floorf(game->foods[i].pos.x * CELL_WIDTH + CELL_WIDTH * 0.5f),
                (int) floorf(game->foods[i].pos.y * CELL_HEIGHT + CELL_HEIGHT * 0.5f),
                (int) floorf(fminf(CELL_WIDTH, CELL_HEIGHT) * 0.5f - FOOD_PADDING),
                FOOD_COLOR);
        }
    }

    for (size_t i = 0; i < WALLS_COUNT; ++i) {
        SDL_FRect rect = {
            (int) floorf(game->walls[i].pos.x * CELL_WIDTH + AGENT_PADDING),
            (int) floorf(game->walls[i].pos.y * CELL_HEIGHT + AGENT_PADDING),
            (int) floorf(CELL_WIDTH) - 2*AGENT_PADDING,
            (int) floorf(CELL_HEIGHT) -2* AGENT_PADDING,
        };

        SDL_SetRenderDrawColor(renderer, HEX_COLOR(WALL_COLOR));

        SDL_RenderFillRect(renderer, &rect);
    }
}

void filledTriangleColor(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, float x3, float y3, Uint32 color) {
    SDL_SetRenderDrawColor(renderer, HEX_COLOR(color));

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

void filledCircleRGBA(SDL_Renderer *renderer, int cx, int cy, int r, Uint32 color) {
    SDL_SetRenderDrawColor(renderer, HEX_COLOR(color));

    // Bounding box for the circle
    int min_y = cy - r;
    int max_y = cy + r;

    // Clip to the screen bounds
    min_y = fmaxf(min_y, 0);
    max_y = fminf(max_y, SCREEN_HEIGHT);

    // For each y in the bounding box
    for (int y = min_y; y <= max_y; y++) {
        // Calculate x-range using circle equation: x^2 + y^2 = r^2
        int dy = y - cy;
        if (abs(dy) > r) continue; // Skip if outside circle
        int dx = (int)sqrtf(r * r - dy * dy);
        int x1 = cx - dx;
        int x2 = cx + dx;

        // Clip x coordinates
        x1 = fmax(0, x1);
        x2 = fmin(SCREEN_WIDTH, x2);

        // Draw horizontal line
        if (x1 <= x2) {
            scc(SDL_RenderLine(renderer, x1, y, x2, y));
        }
    }

    // Draw outline
    for (int i = 0; i < 360; i += 1) {
        float rad = i * SDL_PI_D / 180.0f;
        int x = cx + (int)(r * cosf(rad));
        int y = cy + (int)(r * sinf(rad));
        scc(SDL_RenderPoint(renderer, x, y));
    }
}
