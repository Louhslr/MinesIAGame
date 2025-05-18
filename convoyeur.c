#include "convoyeur.h"
#include <SDL.h>
#include <stdlib.h>

#define MAX_EDGES 100
Edge convoyeurs[MAX_EDGES];
int convoyeur_count = 0;

int find(int parent[], int i) {
    if (parent[i] != i)
        parent[i] = find(parent, parent[i]);
    return parent[i];
}

void unite(int parent[], int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    parent[xroot] = yroot;
}

void generate_convoyeurs(Terrain terrains[NB_ROWS][NB_COLS], Material* materials, int material_count, Factory* factories, int factory_count) {
    convoyeur_count = 0;
    int total_nodes = material_count + factory_count;
    int parent[total_nodes];

    for (int i = 0; i < total_nodes; i++) parent[i] = i;

    typedef struct {
        int from, to, distance;
    } Connection;

    Connection connections[MAX_EDGES];
    int connection_count = 0;

    // Générer les connexions matériaux-usines
    for (int i = 0; i < material_count; ++i) {
        for (int j = 0; j < factory_count; ++j) {
            int dx = materials[i].col - factories[j].col;
            int dy = materials[i].row - factories[j].row;
            int dist = dx * dx + dy * dy; // Pas besoin de racine carrée pour la comparaison
            connections[connection_count++] = (Connection){i, material_count + j, dist};
        }
    }

    // Tri des connexions par distance (simple bubble sort ici pour l'exemple)
    for (int i = 0; i < connection_count - 1; ++i) {
        for (int j = 0; j < connection_count - i - 1; ++j) {
            if (connections[j].distance > connections[j + 1].distance) {
                Connection temp = connections[j];
                connections[j] = connections[j + 1];
                connections[j + 1] = temp;
            }
        }
    }

    // Algorithme de Kruskal
    for (int i = 0; i < connection_count && convoyeur_count < total_nodes - 1; ++i) {
        int from = connections[i].from;
        int to = connections[i].to;

        if (find(parent, from) != find(parent, to)) {
            unite(parent, from, to);
            int m = from;
            int f = to - material_count;

            convoyeurs[convoyeur_count].start_row = materials[m].row;
            convoyeurs[convoyeur_count].start_col = materials[m].col;
            convoyeurs[convoyeur_count].end_row = factories[f].row;
            convoyeurs[convoyeur_count].end_col = factories[f].col;
            convoyeur_count++;
        }
    }
}

void render_convoyeurs(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); // Vert pour les convoyeurs
    for (int i = 0; i < convoyeur_count; ++i) {
        SDL_RenderDrawLine(renderer,
                           convoyeurs[i].start_col * CELL_SIZE + CELL_SIZE / 2,
                           convoyeurs[i].start_row * CELL_SIZE + CELL_SIZE / 2,
                           convoyeurs[i].end_col * CELL_SIZE + CELL_SIZE / 2,
                           convoyeurs[i].end_row * CELL_SIZE + CELL_SIZE / 2);
    }
}
