#ifndef MATERIALS_H
#define MATERIALS_H
#include <SDL.h>
#include "terrain.h"

#define MAX_MATERIALS 7

typedef struct {
    int row, col;
} Material;

void init_materials(Material* materials, Terrain terrains[NB_ROWS][NB_COLS]);
void draw_materials(SDL_Renderer* renderer, Material* materials, int count, int cell_size);

#endif
