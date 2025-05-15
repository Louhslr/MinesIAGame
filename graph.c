#include "graph.h"
#include <limits.h>

typedef struct {
    int x, y, dist;
} Node;

int is_valid(int x, int y) {
    return x >= 0 && y >= 0 && x < NB_COLS && y < NB_ROWS;
}

void compute_dijkstra(Train* train, int target_x, int target_y, int grid[NB_ROWS][NB_COLS]) {
    int dist[NB_ROWS][NB_COLS];
    int visited[NB_ROWS][NB_COLS] = {0};

    for (int i = 0; i < NB_ROWS; i++)
        for (int j = 0; j < NB_COLS; j++)
            dist[i][j] = INT_MAX;

    dist[train->row][train->col] = 0;

    for (int step = 0; step < NB_ROWS * NB_COLS; step++) {
        int min_dist = INT_MAX;
        int ux = -1, uy = -1;

        for (int i = 0; i < NB_ROWS; i++) {
            for (int j = 0; j < NB_COLS; j++) {
                if (!visited[i][j] && dist[i][j] < min_dist) {
                    min_dist = dist[i][j];
                    uy = i;
                    ux = j;
                }
            }
        }

        if (ux == -1 || uy == -1) break;
        visited[uy][ux] = 1;

        int dirs[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        for (int d = 0; d < 4; d++) {
            int vx = ux + dirs[d][0];
            int vy = uy + dirs[d][1];

            if (is_valid(vx, vy) && !visited[vy][vx] && grid[vy][vx] == 0) {
                if (dist[uy][ux] + 1 < dist[vy][vx]) {
                    dist[vy][vx] = dist[uy][ux] + 1;
                }
            }
        }
    }

    // Déplacement du train vers le prochain nœud sur le chemin le plus court
    int cx = train->col;
    int cy = train->row;
    while (cx != target_x || cy != target_y) {
        int min_dist = INT_MAX;
        int nx = cx, ny = cy;
        int dirs[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        for (int d = 0; d < 4; d++) {
            int vx = cx + dirs[d][0];
            int vy = cy + dirs[d][1];
            if (is_valid(vx, vy) && dist[vy][vx] < min_dist) {
                min_dist = dist[vy][vx];
                nx = vx;
                ny = vy;
            }
        }
        if (nx == cx && ny == cy) break; // Bloqué
        cx = nx;
        cy = ny;
        train->col = cx;
        train->row = cy;
        SDL_Delay(100); // Animation du déplacement
    }
}
