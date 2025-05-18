#ifndef CONVOYEUR_H
#define CONVOYEUR_H

#include "factory.h"
#include "materials.h"

typedef struct {
    int start_row, start_col;
    int end_row, end_col;
} Edge;


void generate_convoyeurs(Terrain terrains[NB_ROWS][NB_COLS], Material* materials, int material_count, Factory* factories, int factory_count);
void render_convoyeurs(SDL_Renderer* renderer);

#endif
