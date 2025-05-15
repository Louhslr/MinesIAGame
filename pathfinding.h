#ifndef PATHFINDING_H
#define PATHFINDING_H
#include "train.h"
#include "terrain.h"

void compute_dijkstra(Train* train, int target_col, int target_row, Terrain terrains[NB_ROWS][NB_COLS]);

#endif
