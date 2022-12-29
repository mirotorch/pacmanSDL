#ifndef FIELD
#define FIELD

#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "input.h"

typedef struct Block
{
    SDL_Rect rectangle;
    int type;
    bool collected;
    SDL_Texture* texture;
} Block;

typedef struct Field
{
    SDL_Point beginning;
    SDL_Point size;
    SDL_Rect* exit;
    int blockSize;
    unsigned long int randTimer;
    int scorepointsLeft;
    Block** blocks;
} Field;

void field_init(InputField* input, Field* field, int windowWidth, int windowHeight, int blockSize);
void count_scorepoints(Field* field);

void load_textures(Field* field, SDL_Renderer* ren);
void draw_field(SDL_Renderer* ren, Field* field);

SDL_Point get_start_position(Field* field);
int get_wall_type(int y, int x, SDL_Point size, Block** blocks);
int get_gate_type(int y, int x, SDL_Point size, Block** blocks);

void free_field(Field* field);

bool listcmpr(int* ls1, int* ls2, int size);


#endif