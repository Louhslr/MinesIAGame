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
            materials[placed].type = rand() % 3; // 0 = CUIVRE, 1 = ARGENT, 2 = DIAMANT
            placed++;
        }
    }
}

void draw_materials(SDL_Renderer* renderer, Material* materials, int count, int cell_size) {
    for (int i = 0; i < count; ++i) {
        int x = materials[i].col * cell_size + cell_size / 2;
        int y = materials[i].row * cell_size + cell_size / 2;

        switch (materials[i].type) {
            case CUIVRE: // Rond orange
                SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
                SDL_Rect rond = { x - 5, y - 5, 10, 10 };
                SDL_RenderFillRect(renderer, &rond);
                break;

            case ARGENT: // Carré gris clair
                SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
                SDL_Rect carre = { x - 5, y - 5, 10, 10 };
                SDL_RenderFillRect(renderer, &carre);
                break;

            case DIAMANT: // Triangle bleu clair
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                SDL_Point triangle[4] = {
                        { x, y - 5 },
                        { x - 5, y + 5 },
                        { x + 5, y + 5 },
                        { x, y - 5 }
                };
                SDL_RenderDrawLines(renderer, triangle, 4);
                break;
        }
    }
}
