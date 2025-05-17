#include "train.h"
#include "materials.h"  // Si Inventory est défini ici
#include <SDL.h>

void init_train(Train* train, Terrain terrains[NB_ROWS][NB_COLS]) {
    do {
        train->row = rand() % NB_ROWS;
        train->col = rand() % NB_COLS;
    } while (terrains[train->row][train->col] != HERBE);

    train->path_length = 0;
    train->current_step = 0;
}

void draw_train(SDL_Renderer* renderer, Train* train, int cell_size) {
    int x = train->col * cell_size;
    int y = train->row * cell_size;

    SDL_Rect body = { x + cell_size / 3, y + cell_size / 4, cell_size / 2, cell_size / 2 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &body);

    SDL_Rect chimney = { x + cell_size / 4, y, cell_size / 4, cell_size / 2 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &chimney);
}

void move_train(Train* train) {
    if (train->current_step < train->path_length) {
        train->col = train->path[train->current_step][0];
        train->row = train->path[train->current_step][1];
        train->current_step++;
    }
}