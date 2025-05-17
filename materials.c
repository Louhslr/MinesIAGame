#include "materials.h"
#include "terrain.h"
#include <stdlib.h>
#include <SDL_ttf.h>

void init_materials(Material* materials, Terrain terrains[NB_ROWS][NB_COLS]) {
    int placed = 0;
    while (placed < MAX_MATERIALS) {
        int row = rand() % NB_ROWS;
        int col = rand() % NB_COLS;

        if (terrains[row][col] == HERBE) {  // Vérification correcte
            materials[placed].row = row;
            materials[placed].col = col;
            materials[placed].type = rand() % 3;  // 0: cuivre, 1: argent, 2: diamant
            placed++;
        }
    }
}

void draw_materials(SDL_Renderer* renderer, Material* materials, int count, int cell_size) {
    for (int i = 0; i < count; ++i) {
        SDL_Rect rect = { materials[i].col * cell_size + cell_size / 4, materials[i].row * cell_size + cell_size / 4, cell_size / 2, cell_size / 2 };
        switch (materials[i].type) {
            case 0: SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); break;  // Cuivre: Orange
            case 1: SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); break; // Argent: Gris clair
            case 2: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break;   // Diamant: Cyan
        }
        SDL_RenderFillRect(renderer, &rect);
    }
}

// Ajoute cette fonction dans materials.c
void draw_inventory(SDL_Renderer* renderer, Inventory* inv, int screen_width, int screen_height) {
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 22);
    if (!font) return;

    SDL_Color white = {255, 255, 255, 255};
    char buffer[50];
    int x = screen_width - 190;  // Ajuste si nécessaire
    int y = 40;

    sprintf(buffer, "Cuivre: %d", inv->cuivre);
    SDL_Surface* surface = TTF_RenderText_Solid(font, buffer, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    y += 30;
    sprintf(buffer, "Argent: %d", inv->argent);
    surface = TTF_RenderText_Solid(font, buffer, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect.y = y;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    y += 30;
    sprintf(buffer, "Diamant: %d", inv->diamant);
    surface = TTF_RenderText_Solid(font, buffer, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect.y = y;
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void spawn_material(Material materials[], Terrain terrains[NB_ROWS][NB_COLS]) {
    int attempts = 0;
    while (attempts < 1000) {
        int row = rand() % NB_ROWS;
        int col = rand() % NB_COLS;

        if (terrains[row][col] == HERBE) {  // Vérification correcte
            int occupied = 0;
            for (int i = 0; i < MAX_MATERIALS; ++i) {
                if (materials[i].row == row && materials[i].col == col) {
                    occupied = 1;
                    break;
                }
            }

            if (!occupied) {
                for (int i = 0; i < MAX_MATERIALS; ++i) {
                    if (materials[i].row == -1 && materials[i].col == -1) {
                        materials[i].row = row;
                        materials[i].col = col;
                        materials[i].type = rand() % 3; // 0: cuivre, 1: argent, 2: diamant
                        return;
                    }
                }
            }
        }
        attempts++;
    }
}


