#include "pacman_sprite.h"


void pacman_init(Pacman* pacman, SDL_Point startPosition, int blockSize)
{
    pacman->lives = 3;
    pacman->score = 0;
    pacman->tabletTimer = 0;
    pacman->mouthTimer = 0;
    pacman->currentAngle = 0;
    pacman->openMouth = false;
    pacman->rect.x = startPosition.x;
    pacman->rect.y = startPosition.y;
    pacman->spawnPoint.x = startPosition.x;
    pacman->spawnPoint.y = startPosition.y;
    pacman->rect.h = blockSize;
    pacman->rect.w = blockSize;
    pacman->direction.x = 0;
    pacman->direction.y = 0;
    pacman->surfaceBase = SDL_LoadBMP("src/Pacman.bmp");
    pacman->surfaceOpenMouth = SDL_LoadBMP("src/PacmanOpenMouth.bmp");
    int colorkey = SDL_MapRGB(pacman->surfaceBase->format, 0, 0, 0);
    SDL_SetColorKey(pacman->surfaceBase, SDL_TRUE, colorkey);
    SDL_SetColorKey(pacman->surfaceOpenMouth, SDL_TRUE, colorkey);
}

void pacman_check_point(Pacman *pacman, Field *field)
{
    for (int i = 0; i < field->size.y; i++)
    {
        for (int k = 0; k < field->size.x; k++)
        {
            if (SDL_HasIntersection(&pacman->rect, &field->blocks[i][k].rectangle))
            {
                if (field->blocks[i][k].type == 2 && field->blocks[i][k].collected == false)
                {
                    pacman->tabletTimer += 300;
                    pacman->score += 50;
                }
                else if (field->blocks[i][k].type == 0 && field->blocks[i][k].collected == false)
                    pacman->score += 20;
                field->blocks[i][k].collected = true;
                break;
            }
        }
    }
}

bool pacman_check_walls(Pacman* pacman, Field* field)
{
    for (int i = 0; i < field->size.y; i++)
    {
        for (int k = 0; k < field->size.x; k++)
        {
            if(SDL_HasIntersection(&pacman->rect, &field->blocks[i][k].rectangle) 
                && (field->blocks[i][k].type == 1 || field->blocks[i][k].type == 4))
            {
                return true;
            }
        }
    }
    return false;
}

void pacman_move(Pacman* pacman, Field* field)
{
    if (pacman->futureDirection.x != 0 || pacman->futureDirection.y != 0)
    {
        pacman->rect.x += pacman->futureDirection.x;
        pacman->rect.y += pacman->futureDirection.y;
        if (pacman_check_walls(pacman, field))
        {
            pacman->rect.x -= pacman->futureDirection.x;
            pacman->rect.y -= pacman->futureDirection.y;
        }
        else 
        {
            pacman->direction.x = pacman->futureDirection.x;
            pacman->direction.y = pacman->futureDirection.y;
            pacman->futureDirection.x = 0;
            pacman->futureDirection.y = 0;
        }
    }
    pacman->rect.x += pacman->direction.x;
    pacman->rect.y += pacman->direction.y;
    if (pacman_check_walls(pacman, field))
    {
        pacman->rect.x -= pacman->direction.x;
        pacman->rect.y -= pacman->direction.y;
        pacman->direction.x = 0;
        pacman->direction.y = 0;
    }
    if (pacman->mouthTimer == 0)
    {
        pacman->openMouth = (pacman->openMouth == false) ? true : false;
        pacman->mouthTimer = 25;
    }
    if (!(pacman->direction.x == 0 && pacman->direction.y == 0))
        pacman->mouthTimer--;
    pacman_check_point(pacman, field);
    pacman_check_borders(pacman, field);
}


void pacman_check_borders(Pacman* pacman, Field* field)
{
    SDL_Point currentPos = {pacman->rect.x, pacman->rect.y};
    bool transfer = false;
    if (pacman->rect.x < field->beginning.x)
    {
        pacman->rect.x = field->beginning.x + field->size.x * field->blockSize + 1;
        transfer = true;
    }
    else if (pacman->rect.x > field->beginning.x + field->size.x * field->blockSize)
    {
        pacman->rect.x  = field->beginning.x + 1;
        transfer = true;
    }
    else if (pacman->rect.y < field->beginning.y)
    {
        pacman->rect.y = field->beginning.y + field->size.y * field->blockSize + 1;
        transfer = true;
    }
    else if (pacman->rect.y > field->beginning.y + field->size.y * field->blockSize)
    {
        pacman->rect.y = field->beginning.y + 1;
        transfer = true;
    }
    if (transfer)
    {
        if (pacman_check_walls(pacman, field))
        {
            pacman->rect.x = currentPos.x;
            pacman->rect.y = currentPos.y;
            pacman->direction.x = 0;
            pacman->direction.y = 0;
        }
    }
}




void pacman_handle_keyboard(Pacman* pacman, SDL_KeyboardEvent* key)
{
    switch (key->keysym.sym)
    {
        case SDLK_DOWN:
            pacman->futureDirection.x = 0;
            pacman->futureDirection.y = 1;
            break;
        case SDLK_UP:
            pacman->futureDirection.x = 0;
            pacman->futureDirection.y = -1;
            break;
        case SDLK_LEFT:
            pacman->futureDirection.x = -1;
            pacman->futureDirection.y = 0;
            break;
        case SDLK_RIGHT:
            pacman->futureDirection.x = 1;
            pacman->futureDirection.y = 0;
            break;
    }
}


void pacman_render(Pacman* pacman, SDL_Renderer* ren)
{
    if(pacman->openMouth == false || pacman->mouthTimer > 20)
    {
        SDL_Texture* textureBase = SDL_CreateTextureFromSurface(ren, pacman->surfaceBase);
        SDL_RenderCopy(ren, textureBase, NULL, &pacman->rect);
        SDL_DestroyTexture(textureBase);
    }
    else
    {
        SDL_Texture* textureOpenMouth = SDL_CreateTextureFromSurface(ren, pacman->surfaceOpenMouth);
        if (pacman->direction.x > 0)
        {
            pacman-> currentAngle = 0;
        }
        else if (pacman->direction.x < 0)
        {
            pacman->currentAngle = 180;
        }
        else if (pacman->direction.y > 0)
        {
            pacman->currentAngle = 90;
        }
        else if (pacman->direction.y < 0)
        {
            pacman->currentAngle = 270;
        }
        SDL_RenderCopyEx(ren, textureOpenMouth, NULL, &pacman->rect, pacman->currentAngle, NULL, 0);
        SDL_DestroyTexture(textureOpenMouth);
    }
}

void pacman_free(Pacman* pacman)
{
    SDL_FreeSurface(pacman->surfaceBase);
    SDL_FreeSurface(pacman->surfaceOpenMouth);
}