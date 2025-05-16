#ifndef TRAIN_H
#define TRAIN_H
#include <SDL.h>
#include "terrain.h"

typedef struct {
    int row, col;
    int path_length;
    int path[NB_ROWS * NB_COLS][2];  // Chemin calculé avec Dijkstra
    int current_step;
} Train;

void init_train(Train* train, Terrain terrains[NB_ROWS][NB_COLS]);
void draw_train(SDL_Renderer* renderer, Train* train, int cell_size);
void move_train(Train* train);

#endif