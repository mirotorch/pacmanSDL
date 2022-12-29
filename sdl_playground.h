#ifndef SDL_PLAYGROUND_H_
#define SDL_PLAYGROUND_H_

#include <stdbool.h>
#include <SDL2/SDL.h>


bool sdl_playground_init( SDL_Window ** win, SDL_Renderer ** renderer, const int window_width, const int window_height );
void sdl_playground_destroy( SDL_Window * win, SDL_Renderer * renderer );

#endif