#include "score.h"

void print_scores(TTF_Font *font, Pacman *pacman, SDL_Color color,
                  SDL_Renderer *ren, int fieldHeight, int windowWidth, int windowHeight)
{
    char text[12];
    sprintf(text, "Score: %d", pacman->score);
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.w = 400;
    Message_rect.h = 100;
    Message_rect.x = windowWidth / 2 - Message_rect.w / 2;
    Message_rect.y = windowHeight / 2 + fieldHeight / 2;
    SDL_RenderCopy(ren, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void print_final_screen(SDL_Renderer *ren, char *text, int fieldHeight, int windowWidth,
                        int windowHeight, TTF_Font *font, SDL_Color color)
{
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.w = 400;
    Message_rect.h = 100;
    Message_rect.x = windowWidth / 2 - Message_rect.w / 2;
    Message_rect.y = windowHeight / 2 - fieldHeight / 1.2;
    SDL_RenderCopy(ren, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void display_lives(SDL_Renderer* ren, Pacman* pacman, int windowWidth, int windowHeight, int fieldHeight)
{
    SDL_Rect* rects = malloc(sizeof(SDL_Rect) * pacman->lives);
    for(int i = 0; i < pacman->lives; i++)
    {
        rects[i].h = pacman->rect.h;
        rects[i].w = pacman->rect.w;
        rects[i].x = windowWidth / 2 - (rects[i].w * i);
        rects[i].y = windowHeight / 2 + fieldHeight / 2 + 120;
    }

    SDL_Texture *Live = SDL_CreateTextureFromSurface(ren, pacman->surfaceOpenMouth);
    for(int i = 0; i < pacman->lives; i++)
        SDL_RenderCopy(ren, Live, NULL, &rects[i]);
    SDL_DestroyTexture(Live);
}