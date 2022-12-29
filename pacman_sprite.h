#ifndef PACMAN
#define PACMAN

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "field.h"

typedef struct Pacman
{
    int lives;
    int score;
    int tabletTimer;
    SDL_Rect rect;
    bool openMouth;
    int currentAngle;
    int mouthTimer;
    SDL_Surface* surfaceBase;
    SDL_Surface* surfaceOpenMouth;
    SDL_Point direction;
    SDL_Point futureDirection;
    SDL_Point spawnPoint;
} Pacman;

void pacman_init(Pacman* pacman, SDL_Point startPosition, int blockSize);

bool pacman_check_walls(Pacman* pacman, Field* field);
void pacman_check_borders(Pacman* pacman, Field* field);
void pacman_check_point(Pacman* pacman, Field* field);

void pacman_move(Pacman* pacman, Field* field);
void pacman_handle_keyboard(Pacman* pacman, SDL_KeyboardEvent* key);
void pacman_render(Pacman* pacman, SDL_Renderer* ren);

void pacman_free(Pacman* pacman);

#endif