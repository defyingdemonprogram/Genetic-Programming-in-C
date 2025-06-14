#include <stdio.h>
#include <time.h>
#include <SDL3/SDL.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define HEX_COLOR(hex)                      \
    ((hex) >> (3 * 8)) & 0xFF,              \
    ((hex) >> (2 * 8)) & 0xFF,              \
    ((hex) >> (1 * 8)) & 0xFF,              \
    ((hex) >> (0 * 8)) & 0xFF


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
