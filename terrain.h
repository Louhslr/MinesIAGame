#ifndef TERRAIN_H
#define TERRAIN_H

#include <SDL.h> // Ajout indispensable pour SDL_Renderer

#define NB_ROWS 20
#define NB_COLS 25

typedef enum { HERBE, EAU, PIERRE } Terrain;

void generate_terrain(Terrain terrains[NB_ROWS][NB_COLS]);
void draw_terrain(SDL_Renderer* renderer, Terrain terrains[NB_ROWS][NB_COLS], int cell_size);

#endif
