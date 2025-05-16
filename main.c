#include <SDL.h>
#include <stdlib.h>
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

int main(int argc, char* argv[]) {
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

        move_train(&train);  // Avance automatiquement si un chemin a été calculé

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        draw_terrain(renderer, terrains, CELL_SIZE);
        draw_materials(renderer, materials, MAX_MATERIALS, CELL_SIZE);
        draw_train(renderer, &train, CELL_SIZE);

        SDL_RenderPresent(renderer);
        SDL_Delay(50);  // Vitesse du déplacement visible
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}