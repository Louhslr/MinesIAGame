#include "materials.h"
#include <stdlib.h>

void init_materials(Material* materials, Terrain terrains[NB_ROWS][NB_COLS]) {
    int placed = 0;
    while (placed < MAX_MATERIALS) {
        int r = rand() % NB_ROWS;
        int c = rand() % NB_COLS;
        if (terrains[r][c] == HERBE) {
            materials[placed].row = r;
            materials[placed].col = c;
            placed++;
        }
    }
}

void draw_materials(SDL_Renderer* renderer, Material* materials, int count, int cell_size) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < count; ++i) {
        SDL_Rect rect = { materials[i].col * cell_size + cell_size / 4, materials[i].row * cell_size + cell_size / 4, cell_size / 2, cell_size / 2 };
        SDL_RenderFillRect(renderer, &rect);
    }
}
