#include "factory.h"
#include "materials.h"
#include <SDL.h>
#include <stdio.h>
#include <time.h>

extern Inventory inventory;
Factory factories[MAX_FACTORIES];
int factory_count = 0;

// Dépendances pour le tri topologique
int dependencies[4][4] = {
        {0, 0, 0, 0},  // Cuivre
        {0, 0, 0, 0},  // Argent
        {0, 0, 0, 0},  // Diamant
        {1, 1, 1, 0}   // Avancée dépend de Cuivre, Argent et Diamant
};

int has_built_factory(int type) {
    for (int i = 0; i < factory_count; ++i) {
        if (factories[i].type == type) return 1;
    }
    return 0;
}

int can_build(int type) {
    switch (type) {
        case 0: // Usine Cuivre
            return inventory.cuivre >= 2;
        case 1: // Usine Argent
            return inventory.argent >= 2;
        case 2: // Usine Diamant
            return inventory.diamant >= 2;
        case 3: // Usine Avancée
            return inventory.cuivre >= 2 && inventory.argent >= 2 && inventory.diamant >= 2
                   && has_built_factory(0)  // Usine Cuivre présente
                   && has_built_factory(1)  // Usine Argent présente
                   && has_built_factory(2); // Usine Diamant présente
    }
    return 0;
}


int add_factory(int type, Terrain terrains[NB_ROWS][NB_COLS]) {
    if (!can_build(type)) {
        return 0;  // Construction impossible (pas assez de ressources ou conditions non remplies)
    }

    if (factory_count >= MAX_FACTORIES) return 0;

    int attempts = 0;
    while (attempts < 1000) {  // Limite pour éviter une boucle infinie
        int row = rand() % NB_ROWS;
        int col = rand() % NB_COLS;

        int occupied = 0;
        for (int i = 0; i < factory_count; ++i) {
            if (factories[i].row == row && factories[i].col == col) {
                occupied = 1;
                break;
            }
        }

        if (terrains[row][col] == HERBE && !occupied) {
            factories[factory_count].row = row;
            factories[factory_count].col = col;
            factories[factory_count].type = type;
            factory_count++;

            // Déduire les ressources
            if (type == 0) inventory.cuivre -= 2;
            if (type == 1) inventory.argent -= 2;
            if (type == 2) inventory.diamant -= 2;
            if (type == 3) {
                inventory.cuivre -= 2;
                inventory.argent -= 2;
                inventory.diamant -= 2;
            }

            printf("Usine construite !\n");
            return 1;
        }

        attempts++;
    }

    printf("Impossible de trouver une case libre pour l'usine.\n");
    return 0;  // Construction échouée (par exemple, pas de place trouvée)

}

void render_factories(SDL_Renderer* renderer) {
    for (int i = 0; i < factory_count; ++i) {
        SDL_Rect rect = {
                factories[i].col * CELL_SIZE,
                factories[i].row * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE
        };

        // Couleur selon le type
        switch (factories[i].type) {
            case 0: SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255); break;   // Cuivre
            case 1: SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255); break; // Argent
            case 2: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break;   // Diamant
            case 3: SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); break;   // Avancée
        }

        SDL_RenderFillRect(renderer, &rect);

        // Triangle pour les usines avancées
        if (factories[i].type == 3) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(renderer, rect.x, rect.y + CELL_SIZE, rect.x + CELL_SIZE / 2, rect.y);
            SDL_RenderDrawLine(renderer, rect.x + CELL_SIZE / 2, rect.y, rect.x + CELL_SIZE, rect.y + CELL_SIZE);
            SDL_RenderDrawLine(renderer, rect.x, rect.y + CELL_SIZE, rect.x + CELL_SIZE, rect.y + CELL_SIZE);
        }
    }
}
