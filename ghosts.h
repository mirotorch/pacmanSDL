#ifndef GHOSTS
#define GHOSTS

#include "SDL2/SDL.h"
#include "field.h"
#include "pacman_sprite.h"
#include "stdbool.h"
#include "input.h"
#include "stdlib.h"

typedef struct Ghost
{
    SDL_Rect rect;
    SDL_Rect antiLoop[2];
    SDL_Surface* surface;
    SDL_Point respawnPoint;
    int deathTimer;
    int turnTimer;
    int lastDirections[2];
    bool exited;
    SDL_Point direction;
} Ghost;

typedef struct AllGhosts
{
    int ghostsNum;
    Ghost* ghosts;
    SDL_Rect testRectangle;
    SDL_Surface* ghostFeared;
} AllGhosts;

void all_ghosts_init(AllGhosts* allGhosts, Field* field);

void ghosts_render(AllGhosts* allGhosts, SDL_Renderer* ren, int tabletTimer);

bool ghost_check_walls(SDL_Rect* ghost, SDL_Rect* antiLoop, Field* field);
void ghost_check_pacman(AllGhosts* allGhosts, Pacman* pacman, int* cooldown);
void ghost_check_borders(Ghost* ghost, Field* field);

void ghost_move(Ghost* ghost, Field* field, SDL_Rect* testRectangle, Pacman* pacman);
void ghost_turn(Ghost* ghost, Field* field, SDL_Rect* testRectangle);

void all_ghosts_free(AllGhosts* allGhosts);

#endif