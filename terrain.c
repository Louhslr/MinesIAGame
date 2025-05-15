#include "terrain.h"
#include <SDL.h>
#include <stdlib.h>

void generate_terrain(Terrain terrains[NB_ROWS][NB_COLS]) {
    for (int row = 0; row < NB_ROWS; ++row) {
        for (int col = 0; col < NB_COLS; ++col) {
            int r = rand() % 100;
            if (r < 70) terrains[row][col] = HERBE;
            else if (r < 85) terrains[row][col] = EAU;
            else terrains[row][col] = PIERRE;
        }
    }
}

void draw_terrain(SDL_Renderer* renderer, Terrain terrains[NB_ROWS][NB_COLS], int cell_size) {
    for (int row = 0; row < NB_ROWS; ++row) {
        for (int col = 0; col < NB_COLS; ++col) {
            SDL_Rect rect = { col * cell_size, row * cell_size, cell_size, cell_size };

            switch (terrains[row][col]) {
                case HERBE: SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); break;
                case EAU: SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255); break;
                case PIERRE: SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255); break;
            }

            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}
