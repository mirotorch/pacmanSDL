#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "sdl_playground.h"
#include "input.h"
#include "field.h"
#include "score.h"
#include "pacman_sprite.h"
#include "ghosts.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000


int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("wrong path to field\n");
        return -1;
    }

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("src/SpaceMono-Regular.ttf", 24);
    const SDL_Color RGB_WHITE = {255, 255, 255, 0};
    SDL_Surface *surfaceMessage;

    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;

    bool initialized = sdl_playground_init(&win, &ren, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!initialized)
    {
        sdl_playground_destroy(win, ren);
        return -1;
    }
    
    InputField input;
    read_input(argv[1], &input);

    Field field;
    field_init(&input, &field, WINDOW_WIDTH, WINDOW_HEIGHT, 40);
    load_textures(&field, ren);
    free_input_field(&input);
    count_scorepoints(&field);

    Pacman pacman;
    pacman_init(&pacman, get_start_position(&field), 40);

    AllGhosts allGhosts;
    all_ghosts_init(&allGhosts, &field);

    SDL_Event e;
    bool quit = false;
    // cooldown afer being catched by ghost
    int cooldown = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                    pacman_handle_keyboard(&pacman, &e.key);
                    break;
            }
        }

        print_scores(font, &pacman, RGB_WHITE, ren, field.size.y * field.blockSize, WINDOW_WIDTH, WINDOW_HEIGHT);
        display_lives(ren, &pacman, WINDOW_WIDTH, WINDOW_HEIGHT, field.size.y * field.blockSize);
        count_scorepoints(&field);

        draw_field(ren, &field);
        pacman_render(&pacman, ren);
        ghosts_render(&allGhosts, ren, pacman.tabletTimer);

        if (pacman.lives <= 0)
        {
            cooldown = -1;
            print_final_screen(ren, "GAME OVER", field.size.y * field.blockSize,
                 WINDOW_WIDTH, WINDOW_HEIGHT, font, RGB_WHITE);
        }
        if (field.scorepointsLeft == 0)
        {
            cooldown = -1;
            print_final_screen(ren, "YOU WON", field.size.y * field.blockSize,
                               WINDOW_WIDTH, WINDOW_HEIGHT, font, RGB_WHITE);
        }

        SDL_RenderPresent(ren);

        if (cooldown == 0)
        {
            pacman_move(&pacman, &field);
            for (int i = 0; i < allGhosts.ghostsNum; i++)
            {
                    if (allGhosts.ghosts[i].deathTimer == 0)
                    {
                        ghost_move(&allGhosts.ghosts[i], &field, &allGhosts.testRectangle, &pacman);
                    }
                    else
                        allGhosts.ghosts[i].deathTimer--;
            }
        }
        else
            cooldown--;

        if(pacman.tabletTimer > 0)
        {
            pacman.tabletTimer--;
        }

        ghost_check_pacman(&allGhosts, &pacman, &cooldown);
        SDL_RenderClear(ren);

        if(field.randTimer > 4294967290)
            field.randTimer = 0;
    }

    all_ghosts_free(&allGhosts);
    pacman_free(&pacman);
    free_field(&field);
    sdl_playground_destroy(win, ren);
    TTF_Quit();

    return 0;
}