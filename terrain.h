#ifndef TERRAIN_H
#define TERRAIN_H
#include <SDL.h>

#define NB_ROWS 20
#define NB_COLS 25

typedef enum { HERBE, EAU, PIERRE, ARBRE } Terrain;

void generate_terrain(Terrain terrains[NB_ROWS][NB_COLS]);
void draw_terrain(SDL_Renderer* renderer, Terrain terrains[NB_ROWS][NB_COLS], int cell_size);

#endif
