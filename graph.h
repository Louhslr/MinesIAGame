#ifndef GRAPH_H
#define GRAPH_H
#define NB_ROWS 15
#define NB_COLS 20


#include "train.h"

void compute_dijkstra(Train* train, int target_x, int target_y, int grid[NB_ROWS][NB_COLS]);

#endif
