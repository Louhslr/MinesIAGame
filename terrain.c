#include "terrain.h"
#include <SDL.h>
#include <stdlib.h>
#include "materials.h"  // Pour le type Inventory
#include <SDL_ttf.h>    // Pour les fonctions TTF



void generate_terrain(Terrain terrains[NB_ROWS][NB_COLS]) {
    for (int row = 0; row < NB_ROWS; row++) {
        for (int col = 0; col < NB_COLS; col++) {
            int r = rand() % 100;
            if (r < 70) {
                terrains[row][col] = (rand() % 10 == 0) ? ARBRE : HERBE; // 10% chance for tree on grass
            } else if (r < 85) {
                terrains[row][col] = EAU;
            } else {
                terrains[row][col] = PIERRE;
            }
        }
    }
}

void draw_tree(SDL_Renderer* renderer, int x, int y, int cell_size) {
    int center_x = x + cell_size / 2;
    int base_y = y + cell_size / 2;

    // Tronc
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_Rect trunk = { center_x - 2, base_y + 5, 4, 8 };
    SDL_RenderFillRect(renderer, &trunk);

    // Premier triangle (bas)
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_Point lower_triangle[4] = {
            { center_x, base_y - 2 },
            { center_x - 8, base_y + 8 },
            { center_x + 8, base_y + 8 },
            { center_x, base_y - 2 }
    };
    SDL_RenderDrawLines(renderer, lower_triangle, 4);

    // Deuxième triangle (haut)
    SDL_Point upper_triangle[4] = {
            { center_x, base_y - 10 },
            { center_x - 6, base_y },
            { center_x + 6, base_y },
            { center_x, base_y - 10 }
    };
    SDL_RenderDrawLines(renderer, upper_triangle, 4);
}

void draw_terrain(SDL_Renderer* renderer, Terrain terrains[NB_ROWS][NB_COLS], int cell_size, Inventory* inventory) {
    char buffer[50];
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        return;
    }
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface;
    SDL_Texture* texture;

    // 1. Dessiner la zone noire pour les compteurs
    for (int row = 0; row < 11; ++row) {
        for (int col = NB_COLS - 5; col < NB_COLS; ++col) {
            SDL_Rect rect = { col * cell_size, row * cell_size, cell_size, cell_size };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
            SDL_RenderFillRect(renderer, &rect);
            terrains[row][col] = OBSTACLE_NOIR; // Bloquer le passage sur cette zone
        }
    }

    // Afficher le titre "Compteur de matériaux"
    sprintf(buffer, "Ressources");
    surface = TTF_RenderText_Solid(font, buffer, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect title_rect = { (NB_COLS - 5) * cell_size + 5, 5, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &title_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // 2. Afficher le reste du terrain normalement
    for (int row = 0; row < NB_ROWS; row++) {
        for (int col = 0; col < NB_COLS; col++) {
            if (row < 11 && col >= NB_COLS - 5) continue; // Ne pas redessiner la zone noire

            SDL_Rect rect = { col * cell_size, row * cell_size, cell_size, cell_size };

            switch (terrains[row][col]) {
                case HERBE: SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); break;
                case EAU: SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255); break;
                case PIERRE: SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255); break;
                case ARBRE: SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); break;
            }

            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderDrawRect(renderer, &rect);

            if (terrains[row][col] == ARBRE) {
                draw_tree(renderer, col * cell_size, row * cell_size, cell_size);
            }
        }
    }

    TTF_CloseFont(font);
}

void create_black_zone(Terrain terrains[NB_ROWS][NB_COLS]) {
    for (int row = 0; row < 11; ++row) {
        for (int col = NB_COLS - 5; col < NB_COLS; ++col) {
            terrains[row][col] = OBSTACLE_NOIR;
        }
    }
}
