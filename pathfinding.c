#include "pathfinding.h"
#include <limits.h>

typedef struct {
    int row, col;
    int dist;
} Node;

int is_valid(int row, int col) {
    return row >= 0 && col >= 0 && row < NB_ROWS && col < NB_COLS;
}

void compute_dijkstra(Train* train, int target_col, int target_row, Terrain terrains[NB_ROWS][NB_COLS]) {
    int dist[NB_ROWS][NB_COLS];
    int visited[NB_ROWS][NB_COLS] = {0};

    for (int i = 0; i < NB_ROWS; ++i)
        for (int j = 0; j < NB_COLS; ++j)
            dist[i][j] = INT_MAX;

    dist[train->row][train->col] = 0;

    for (int count = 0; count < NB_ROWS * NB_COLS; ++count) {
        int min_dist = INT_MAX;
        int cur_row = -1, cur_col = -1;

        for (int i = 0; i < NB_ROWS; ++i) {
            for (int j = 0; j < NB_COLS; ++j) {
                if (!visited[i][j] && dist[i][j] < min_dist) {
                    min_dist = dist[i][j];
                    cur_row = i;
                    cur_col = j;
                }
            }
        }

        if (cur_row == -1) break;
        visited[cur_row][cur_col] = 1;

        int dirs[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        for (int d = 0; d < 4; ++d) {
            int new_row = cur_row + dirs[d][0];
            int new_col = cur_col + dirs[d][1];

            if (is_valid(new_row, new_col) && !visited[new_row][new_col] &&
                (terrains[new_row][new_col] == HERBE || terrains[new_row][new_col] == ARBRE)) {
                if (dist[cur_row][cur_col] + 1 < dist[new_row][new_col]) {
                    dist[new_row][new_col] = dist[cur_row][cur_col] + 1;
                }
            }
        }
    }

    // Reconstruction du chemin
    train->path_length = dist[target_row][target_col];
    train->current_step = 0;

    int cur_row = target_row;
    int cur_col = target_col;

    for (int step = dist[target_row][target_col] - 1; step >= 0; --step) {
        train->path[step][0] = cur_col;
        train->path[step][1] = cur_row;

        int dirs[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        for (int d = 0; d < 4; ++d) {
            int new_row = cur_row + dirs[d][0];
            int new_col = cur_col + dirs[d][1];

            if (is_valid(new_row, new_col) && dist[new_row][new_col] == step) {
                cur_row = new_row;
                cur_col = new_col;
                break;
            }
        }
    }
}