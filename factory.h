#ifndef FACTORY_H
#define FACTORY_H
#define CELL_SIZE 40


#include "terrain.h"

typedef struct {
    int row;
    int col;
    int type; // 0: Cuivre, 1: Argent, 2: Diamant, 3: Avancée
} Factory;
extern int factory_count;
#define MAX_FACTORIES 50
int has_built_factory(int type);


int add_factory(int type, Terrain terrains[NB_ROWS][NB_COLS]);
void render_factories(SDL_Renderer* renderer);
int can_build(int type);

#endif
