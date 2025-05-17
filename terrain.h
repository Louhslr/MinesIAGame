#ifndef TERRAIN_H
#define TERRAIN_H

#include <SDL.h>
#include "materials.h"  // OK car maintenant Terrain est défini

#define NB_ROWS 20
#define NB_COLS 25


void generate_terrain(Terrain terrains[NB_ROWS][NB_COLS]);
void draw_terrain(SDL_Renderer* renderer, Terrain terrains[NB_ROWS][NB_COLS], int cell_size, Inventory* inventory);
void create_black_zone(Terrain terrains[NB_ROWS][NB_COLS]);

#endif