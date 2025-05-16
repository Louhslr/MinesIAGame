#ifndef MATERIALS_H
#define MATERIALS_H

#include <SDL.h>
#include "terrain.h"

#define MAX_MATERIALS 15

typedef enum { CUIVRE, ARGENT, DIAMANT } MaterialType;

typedef struct {
    int row;
    int col;
    MaterialType type;
} Material;

void init_materials(Material* materials, Terrain terrains[NB_ROWS][NB_COLS]);
void draw_materials(SDL_Renderer* renderer, Material* materials, int count, int cell_size);

#endif
