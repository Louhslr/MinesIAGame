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

void draw_inventory(SDL_Renderer* renderer, Inventory* inv, int screen_width, int screen_height) {
    int base_x = 10;
    int base_y = screen_height - 60;

    SDL_Rect rect_cuivre = { base_x, base_y, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); // Couleur cuivre
    SDL_RenderFillRect(renderer, &rect_cuivre);

    SDL_Rect rect_argent = { base_x + 30, base_y, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); // Couleur argent
    SDL_RenderFillRect(renderer, &rect_argent);

    SDL_Rect rect_diamant = { base_x + 60, base_y, 20, 20 };
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Couleur diamant
    SDL_RenderFillRect(renderer, &rect_diamant);

    // Pour l'instant, pas d'affichage numérique, juste les icônes des matériaux.
}