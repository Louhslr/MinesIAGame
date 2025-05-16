#include <SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "train.h"
#include "materials.h"
#include "terrain.h"
#include "pathfinding.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define NB_COLS 25
#define NB_ROWS 20
#define CELL_SIZE 40

Train train;
Material materials[MAX_MATERIALS];
Terrain terrains[NB_ROWS][NB_COLS];
Inventory inventory = {0, 0, 0};


void handle_click(int x, int y) {
    int col = x / CELL_SIZE;
    int row = y / CELL_SIZE;
    for (int i = 0; i < MAX_MATERIALS; ++i) {
        if (materials[i].col == col && materials[i].row == row) {
            compute_dijkstra(&train, col, row, terrains);
            break;
        }
    }
}

void check_material_collection() {
    for (int i = 0; i < MAX_MATERIALS; ++i) {
        if (train.col == materials[i].col && train.row == materials[i].row) {
            switch (materials[i].type) {
                case 0: inventory.cuivre++; break;
                case 1: inventory.argent++; break;
                case 2: inventory.diamant++; break;
            }
            // Supprimer le matériau de la carte après récupération
            materials[i].col = -1;
            materials[i].row = -1;
        }
    }
}

int main(int argc, char* argv[]) {
    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        return -1;
    }


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Graphe & IA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    srand(time(NULL));
    generate_terrain(terrains);
    init_train(&train, terrains);
    init_materials(materials, terrains);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            else if (event.type == SDL_MOUSEBUTTONDOWN) handle_click(event.button.x, event.button.y);
        }

        move_train(&train);
        check_material_collection();  // Vérifie à chaque déplacement si un matériau est collecté

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        draw_terrain(renderer, terrains, CELL_SIZE);
        draw_materials(renderer, materials, MAX_MATERIALS, CELL_SIZE);
        draw_train(renderer, &train, CELL_SIZE);
        draw_inventory(renderer, &inventory, SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
