#ifndef FACTORY_H
#define FACTORY_H

#define MAX_BUILDINGS 50

typedef struct {
    int row;
    int col;
    int type;  // 0: Argent, 1: Diamant, 2: Usine Avancée
} Building;

extern Building buildings[MAX_BUILDINGS];
extern int building_count;

#endif
