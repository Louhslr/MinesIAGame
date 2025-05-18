#include <SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "train.h"
#include "materials.h"
#include "terrain.h"
#include "pathfinding.h"
#include "factory.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define NB_COLS 25
#define NB_ROWS 20
#define CELL_SIZE 40
#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 30
#define BUTTON_MARGIN 10

SDL_Rect buttons[4];
const char* button_labels[4] = { "Usine Cuivre", "Usine Argent", "Usine Diamant", "Usine Avancee" };

Train train;
Material materials[MAX_MATERIALS];
Terrain terrains[NB_ROWS][NB_COLS];
Inventory inventory = {0, 0, 0};
char last_action_message[100] = "Cliquez sur un materiau pour commencer.";

// Gestion éclair
Uint32 last_lightning_time = 0;
const Uint32 LIGHTNING_INTERVAL = 15000;  // 15 secondes en millisecondes

int game_started = 0;
SDL_Rect start_button = { (SCREEN_WIDTH - 300) / 2, (SCREEN_HEIGHT - 80) / 2, 300, 80 };

Uint32 start_time = 0;

int game_over = 0;  // 0 = en cours, 1 = gagné, -1 = perdu


void handle_click(int x, int y) {
    int col = x / CELL_SIZE;
    int row = y / CELL_SIZE;
    for (int i = 0; i < MAX_MATERIALS; ++i) {
        if (materials[i].col == col && materials[i].row == row) {
            compute_dijkstra(&train, col, row, terrains);
            break;
        }
    }
}

void check_material_collection() {
    for (int i = 0; i < MAX_MATERIALS; ++i) {
        if (materials[i].col != -1 && materials[i].row != -1 &&
            train.col == materials[i].col && train.row == materials[i].row) {
            switch (materials[i].type) {
                case 0: inventory.cuivre++; break;
                case 1: inventory.argent++; break;
                case 2: inventory.diamant++; break;
            }
            materials[i].col = -1;
            materials[i].row = -1;
            spawn_material(materials, terrains);
        }
    }
}

void render_text_centered(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect rect) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_x = rect.x + (rect.w - surface->w) / 2;
    int text_y = rect.y + (rect.h - surface->h) / 2;
    SDL_Rect dest = {text_x, text_y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render_wrapped_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect rect, int line_spacing) {
    SDL_Color color = {255, 255, 255, 255};
    char buffer[256];
    strncpy(buffer, text, sizeof(buffer));

    char* line = strtok(buffer, " ");
    char current_line[256] = "";
    int y_offset = 0;

    while (line) {
        char temp_line[256];
        snprintf(temp_line, sizeof(temp_line), "%s %s", current_line, line);

        SDL_Surface* surface = TTF_RenderText_Solid(font, temp_line, color);
        if (surface->w > rect.w) {
            SDL_Surface* current_surface = TTF_RenderText_Solid(font, current_line, color);
            SDL_Texture* current_texture = SDL_CreateTextureFromSurface(renderer, current_surface);

            int text_x = rect.x + (rect.w - current_surface->w) / 2;
            int text_y = rect.y + y_offset;
            SDL_Rect dest = {text_x, text_y, current_surface->w, current_surface->h};
            SDL_RenderCopy(renderer, current_texture, NULL, &dest);

            SDL_FreeSurface(current_surface);
            SDL_DestroyTexture(current_texture);

            strcpy(current_line, line);
            y_offset += current_surface->h + line_spacing;
        } else {
            strcpy(current_line, temp_line);
        }

        SDL_FreeSurface(surface);
        line = strtok(NULL, " ");
    }

    SDL_Surface* last_surface = TTF_RenderText_Solid(font, current_line, color);
    SDL_Texture* last_texture = SDL_CreateTextureFromSurface(renderer, last_surface);

    int text_x = rect.x + (rect.w - last_surface->w) / 2;
    int text_y = rect.y + y_offset;
    SDL_Rect dest = {text_x, text_y, last_surface->w, last_surface->h};
    SDL_RenderCopy(renderer, last_texture, NULL, &dest);

    SDL_FreeSurface(last_surface);
    SDL_DestroyTexture(last_texture);
}

int main(int argc, char* argv[]) {
    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        return -1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Font* font = TTF_OpenFont("ARIALI 1.TTF", 18);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("Graphe & IA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    srand(time(NULL));
    generate_terrain(terrains);
    create_black_zone(terrains);
    init_train(&train, terrains);
    init_materials(materials, terrains);

    int running = 1;
    SDL_Event event;

    for (int i = 0; i < 4; ++i) {
        buttons[i].x = SCREEN_WIDTH - BUTTON_WIDTH - 45;
        buttons[i].y = 150 + i * (BUTTON_HEIGHT + BUTTON_MARGIN);
        buttons[i].w = BUTTON_WIDTH;
        buttons[i].h = BUTTON_HEIGHT;
    }

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                if (!game_started) {
                    if (mx >= start_button.x && mx <= start_button.x + start_button.w &&
                        my >= start_button.y && my <= start_button.y + start_button.h) {

                        // Reset de la partie
                        game_started = 1;
                        game_over = 0;
                        start_time = SDL_GetTicks();
                        inventory.cuivre = 0;
                        inventory.argent = 0;
                        inventory.diamant = 0;
                        factory_count = 0;  // Remet à zéro le nombre d'usines construites
                        generate_terrain(terrains);
                        create_black_zone(terrains);
                        init_train(&train, terrains);
                        init_materials(materials, terrains);
                        snprintf(last_action_message, sizeof(last_action_message), "Partie commencee !");

                    }
                    continue;
                }

                if (game_over != 0) continue;

                int button_clicked = 0;
                for (int i = 0; i < 4; ++i) {
                    if (mx >= buttons[i].x && mx <= buttons[i].x + buttons[i].w &&
                        my >= buttons[i].y && my <= buttons[i].y + buttons[i].h) {

                        if (add_factory(i, terrains)) {
                            snprintf(last_action_message, sizeof(last_action_message), "%s construite !", button_labels[i]);
                        } else {
                            snprintf(last_action_message, sizeof(last_action_message), "Pas assez de ressources pour %s.", button_labels[i]);
                        }
                        button_clicked = 1;
                        break;
                    }
                }

                if (!button_clicked) {
                    handle_click(mx, my);
                    snprintf(last_action_message, sizeof(last_action_message), "Collecte de materiau !");
                }
            }
        }

        move_train(&train);
        check_material_collection();

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        if (!game_started) {
            if (game_over == -1) {
                SDL_Rect msg_rect = { start_button.x, start_button.y - 60, start_button.w, 40 };
                render_text_centered(renderer, font, "GAME OVER", msg_rect);
            } else if (game_over == 1) {
                SDL_Rect msg_rect = { start_button.x, start_button.y - 60, start_button.w, 40 };
                render_text_centered(renderer, font, "BRAVO ! VOUS AVEZ REUSSI", msg_rect);
            }

            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &start_button);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &start_button);
            render_text_centered(renderer, font, "Lancer la partie", start_button);

            SDL_Rect instruction_rect = { start_button.x, start_button.y + start_button.h + 20, start_button.w, 100 };
            render_wrapped_text(renderer, font,
                                "Objectif : Construisez les 4 usines en moins d'une minute ! "
                                "Cliquez sur les ressources et construisez vite avant la fin du temps.",
                                instruction_rect, 5);

            SDL_RenderPresent(renderer);
            continue;
        }

        draw_terrain(renderer, terrains, CELL_SIZE, &inventory);
        draw_materials(renderer, materials, MAX_MATERIALS, CELL_SIZE);
        draw_train(renderer, &train, CELL_SIZE);
        draw_inventory(renderer, &inventory, SCREEN_WIDTH, SCREEN_HEIGHT);
        render_factories(renderer);

        for (int i = 0; i < 4; ++i) {
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &buttons[i]);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &buttons[i]);
            render_text_centered(renderer, font, button_labels[i], buttons[i]);
        }

        SDL_Rect message_rect = {
                buttons[0].x,
                buttons[3].y + BUTTON_HEIGHT + 2 * BUTTON_MARGIN,
                BUTTON_WIDTH,
                60
        };
        render_wrapped_text(renderer, font, last_action_message, message_rect, 5);

        // Gestion du timer et victoire/défaite
        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed = (current_time - start_time) / 1000;
        Uint32 remaining = (60 > elapsed) ? 60 - elapsed : 0;

        char timer_text[50];
        snprintf(timer_text, sizeof(timer_text), "Temps restant : %02d:%02d", remaining / 60, remaining % 60);

        SDL_Rect timer_rect = {
                SCREEN_WIDTH - BUTTON_WIDTH - 30,
                SCREEN_HEIGHT - 400,
                BUTTON_WIDTH,
                30
        };
        render_text_centered(renderer, font, timer_text, timer_rect);

        // Vérification des 4 usines construites
        int built_count = 0;
        for (int i = 0; i < 4; ++i) {
            if (has_built_factory(i)) built_count++;
        }

        if (built_count == 4 && game_over == 0) {
            snprintf(last_action_message, sizeof(last_action_message), "Bravo ! Vous avez réussi !");
            game_over = 1;
            game_started = 0;
        }

        if (remaining == 0 && game_over == 0) {
            snprintf(last_action_message, sizeof(last_action_message), "Temps écoulé !");
            game_over = -1;
            game_started = 0;
        }

        SDL_RenderPresent(renderer);

        if (current_time - last_lightning_time >= LIGHTNING_INTERVAL) {
            last_lightning_time = current_time;
            if (inventory.cuivre > 0) inventory.cuivre--;
            if (inventory.argent > 0) inventory.argent--;
            if (inventory.diamant > 0) inventory.diamant--;

            snprintf(last_action_message, sizeof(last_action_message), "Un éclair ! -1 de chaque ressource.");
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(100);
        }

        SDL_Delay(50);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
