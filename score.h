#ifndef SCORE
#define SCORE

#include "pacman_sprite.h"
#include "SDL2/SDL_ttf.h"

void print_scores(TTF_Font* font, Pacman* pacman, SDL_Color color, 
    SDL_Renderer* ren, int fieldHeight, int windowWidth, int windowHeight);

void print_final_screen(SDL_Renderer* ren, char* text, int fieldHeight,
     int windowWidth, int windowHeight, TTF_Font* font,  SDL_Color color);

void display_lives(SDL_Renderer* ren, Pacman* pacman, int windowWidth, int windowHeight, int fieldHeight);

#endif