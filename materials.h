#ifndef MATERIALS_H
#define MATERIALS_H

#include <SDL.h>

#define MAX_MATERIALS 15

typedef struct {
    int row, col, type;
} Material;

typedef struct {
    int cuivre;
    int argent;
    int diamant;
} Inventory;

typedef enum { HERBE, EAU, PIERRE, ARBRE, OBSTACLE_NOIR } Terrain;

void init_materials(Material* materials, Terrain terrains[20][25]);
void draw_materials(SDL_Renderer* renderer, Material* materials, int count, int cell_size);
void draw_inventory(SDL_Renderer* renderer, Inventory* inv, int screen_width, int screen_height);
void spawn_material(Material materials[], Terrain terrains[20][25]);

#endif
