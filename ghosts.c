#include "ghosts.h"

void all_ghosts_init(AllGhosts* allGhosts, Field* field)
{
    for(int i = 0; i < field->size.y; i++)
    {
        for(int k = 0; k < field->size.x; k++)
        {
            if(field->blocks[i][k].type == 3) allGhosts->ghostsNum++;
        }
    }
    SDL_Point* positionOfRespawn = malloc(sizeof(SDL_Point) * allGhosts->ghostsNum);
    int f = 0;
    for (int i = 0; i < field->size.y; i++)
    {
        for (int k = 0; k < field->size.x; k++)
        {
            if(field->blocks[i][k].type == 3)
            {
                positionOfRespawn[f].x = field->blocks[i][k].rectangle.x;
                positionOfRespawn[f].y = field->blocks[i][k].rectangle.y; 
                f++;
            }
        }
    }
    allGhosts->ghostFeared = SDL_LoadBMP("src/ghostFeared.bmp");
    int colorkey = SDL_MapRGB(allGhosts->ghostFeared->format, 0, 0, 0);
    SDL_SetColorKey(allGhosts->ghostFeared, SDL_TRUE, colorkey);
    allGhosts->ghosts = malloc(sizeof(Ghost) * allGhosts->ghostsNum);
    for(int i = 0; i < allGhosts->ghostsNum; i++)
    {
        allGhosts->ghosts[i].deathTimer = 0 + i * 300;
        allGhosts->ghosts[i].turnTimer = 0;
        allGhosts->ghosts[i].exited = false;

        allGhosts->ghosts[i].rect.w = field->blockSize;
        allGhosts->ghosts[i].rect.h = field->blockSize;

        allGhosts->ghosts[i].antiLoop[0].h = field->blockSize;
        allGhosts->ghosts[i].antiLoop[0].w = field->blockSize;
        allGhosts->ghosts[i].antiLoop[1].h = field->blockSize;
        allGhosts->ghosts[i].antiLoop[1].w = field->blockSize;
        allGhosts->ghosts[i].antiLoop[1].x = field->exit->x;
        allGhosts->ghosts[i].antiLoop[1].y = field->exit->y;

        allGhosts->testRectangle.w = field->blockSize;
        allGhosts->testRectangle.h = field->blockSize;

        allGhosts->ghosts[i].respawnPoint.x = positionOfRespawn[i].x;
        allGhosts->ghosts[i].respawnPoint.y = positionOfRespawn[i].y;
        allGhosts->ghosts[i].rect.x = positionOfRespawn[i].x;
        allGhosts->ghosts[i].rect.y = positionOfRespawn[i].y;

        allGhosts->ghosts[i].lastDirections[0] = 5;
        allGhosts->ghosts[i].lastDirections[1] = 5;

        allGhosts->ghosts[i].direction.x = 0;
        allGhosts->ghosts[i].direction.y = 0;

        char intStr[12];
        char tmp3[30] = "src/";
        int k = i % 5;
        sprintf(intStr, "ghost%d.bmp", k);
        strcat(tmp3, intStr);
        allGhosts->ghosts[i].surface = SDL_LoadBMP(tmp3);
        SDL_SetColorKey(allGhosts->ghosts[i].surface, SDL_TRUE, colorkey);
    }
}

void ghosts_render(AllGhosts* allGhosts, SDL_Renderer* ren, int tabletTimer)
{
    bool noTablet = tabletTimer == 0;
    bool tabletTicking = tabletTimer > 0 && tabletTimer < 120 && tabletTimer % 20 < 10;
    for (int i = 0; i < allGhosts->ghostsNum; i++)
    {
        if(noTablet || tabletTicking || allGhosts->ghosts[i].rect.x == allGhosts->ghosts[i].respawnPoint.x 
            && allGhosts->ghosts[i].rect.y == allGhosts->ghosts[i].respawnPoint.y)
        {
            SDL_Texture *textureBase = SDL_CreateTextureFromSurface(ren, allGhosts->ghosts[i].surface);
            SDL_RenderCopy(ren, textureBase, NULL, &allGhosts->ghosts[i].rect);
            SDL_DestroyTexture(textureBase);
        }
        else
        {
            SDL_Texture *textureFeared = SDL_CreateTextureFromSurface(ren, allGhosts->ghostFeared);
            SDL_RenderCopy(ren, textureFeared, NULL, &allGhosts->ghosts[i].rect);
            SDL_DestroyTexture(textureFeared);
        }
    }
}


bool ghost_check_walls(SDL_Rect* ghost, SDL_Rect* antiLoop, Field* field)
{
    for(int i = 0; i < field->size.y; i++)
    {
        for(int k = 0; k < field->size.x; k++)
        {

            if(SDL_HasIntersection(ghost, &field->blocks[i][k].rectangle) && (field->blocks[i][k].type == 1 
                || field->blocks[i][k].type == 3 || (field->blocks[i][k].type == 4 && !SDL_HasIntersection(antiLoop, field->exit))))
            {
                return true;
            }
        }
    }
    return false;
}



void ghost_turn(Ghost* ghost, Field* field, SDL_Rect* testRectangle)
{
    srand(field->randTimer);
    field->randTimer++;
    int tmp = rand() % 2;
    int turn;
    if(tmp == 0) 
        turn = field->blockSize;
    else if(tmp == 1)
        turn = field->blockSize * -1;
    if(ghost->direction.x == 0)
    {
        testRectangle->x = ghost->rect.x + turn;
        if(ghost_check_walls(testRectangle, &ghost->antiLoop[1], field) == false)
        {
            ghost->direction.y = 0;
            ghost->direction.x = (turn > 0) ? 1 : -1;
            ghost->antiLoop[0] = ghost->antiLoop[1];
        }
        else
        {
            testRectangle->x == ghost->rect.x - turn;
            if (ghost_check_walls(testRectangle, &ghost->antiLoop[1], field) == false)
            {
                ghost->direction.y = 0;
                ghost->direction.x = (turn > 0) ? -1 : 1;
                ghost->antiLoop[0] = ghost->antiLoop[1];
            }
        }
    }
    else if (ghost->direction.y == 0)
    {
        testRectangle->y = ghost->rect.y + turn;
        if(ghost_check_walls(testRectangle, &ghost->antiLoop[1], field) == false)
        {
            ghost->direction.x = 0;
            ghost->direction.y = (turn > 0) ? 1 : -1;
            ghost->antiLoop[0] = ghost->antiLoop[1];
        }
        else
        {
            testRectangle->y == ghost->rect.y - turn;
            if (ghost_check_walls(testRectangle, &ghost->antiLoop[1], field) == false)
            {
                ghost->direction.x = 0;
                ghost->direction.y = (turn > 0) ? -1 : 1;
                ghost->antiLoop[0] = ghost->antiLoop[1];
            }
        }
    }
}


void ghost_move(Ghost* ghost, Field* field, SDL_Rect* testRectangle, Pacman* pacman)
{
    if(!ghost->exited)
    {
        ghost->rect.x = field->exit->x;
        ghost->rect.y = field->exit->y;
        ghost->exited = true;
    }
    if (ghost->direction.x == 0 && ghost->direction.y == 0)
    {
        srand(field->randTimer);
        field->randTimer++;
        int direction = rand() % 4;
        ghost->lastDirections[0] = ghost->lastDirections[1];
        while(ghost->lastDirections[0] == direction)
        {
            direction = rand() % 4;
        }
        ghost->lastDirections[1] = direction;
        switch (direction)
        {
        case 0:
            ghost->direction.x = 1;
            ghost->direction.y = 0;
            break;

        case 1:
            ghost->direction.x = 0;
            ghost->direction.y = 1;
            break;

        case 2:
            ghost->direction.x = -1;
            ghost->direction.y = 0;
            break;
        case 3:
            ghost->direction.x = 0;
            ghost->direction.y = -1;
            break;
        }
    }
    testRectangle->x = ghost->rect.x + ghost->direction.x;
    testRectangle->y = ghost->rect.y + ghost->direction.y;

    if (ghost_check_walls(testRectangle, &ghost->antiLoop[1], field) == true)
    {
        ghost->direction.x = 0;
        ghost->direction.y = 0;
    }
    else
    {
        ghost->rect.x += ghost->direction.x;
        ghost->rect.y += ghost->direction.y;
    }

    if(ghost->turnTimer == 0)
    {
        ghost->antiLoop[1] = ghost->rect;
        if(!SDL_HasIntersection(&ghost->antiLoop[0], &ghost->antiLoop[1]))
        {
            ghost_turn(ghost, field, testRectangle);
            ghost->turnTimer = 10;
        }
    }
    else
        ghost->turnTimer--;
    ghost_check_borders(ghost, field);
}



void ghost_check_borders(Ghost* ghost, Field* field)
{
    SDL_Point currentPos = {ghost->rect.x, ghost->rect.y};
    bool transfer = false;
    if (ghost->rect.x < field->beginning.x)
    {
        ghost->rect.x = field->beginning.x + field->size.x * field->blockSize + 1;
        transfer = true;
    }
    else if (ghost->rect.x > field->beginning.x + field->size.x * field->blockSize)
    {
        ghost->rect.x  = field->beginning.x + 1;
        transfer = true;
    }
    else if (ghost->rect.y < field->beginning.y)
    {
        ghost->rect.y = field->beginning.y + field->size.y * field->blockSize + 1;
        transfer = true;
    }
    else if (ghost->rect.y > field->beginning.y + field->size.y * field->blockSize)
    {
        ghost->rect.y = field->beginning.y + 1;
        transfer = true;
    }
    if (transfer)
    {
        if (ghost_check_walls(&ghost->rect, ghost->antiLoop, field))
        {
            ghost->rect.x = currentPos.x;
            ghost->rect.y = currentPos.y;
            ghost->direction.x = 0;
            ghost->direction.y = 0;
        }
    }
}


void ghost_check_pacman(AllGhosts* allGhosts, Pacman* pacman, int* cooldown)
{
    for (int i = 0; i < allGhosts->ghostsNum; i++)
    {
        if (SDL_HasIntersection(&allGhosts->ghosts[i].rect, &pacman->rect))
        {
            if (pacman->tabletTimer == 0)
            {
                pacman->lives--;
                pacman->rect.x = pacman->spawnPoint.x;
                pacman->rect.y = pacman->spawnPoint.y;
                for (int k = 0; k < allGhosts->ghostsNum; k++)
                {
                    allGhosts->ghosts[k].rect.x = allGhosts->ghosts[k].respawnPoint.x;
                    allGhosts->ghosts[k].rect.y = allGhosts->ghosts[k].respawnPoint.y;
                    allGhosts->ghosts[k].deathTimer = 50 + k * 300;
                    allGhosts->ghosts[k].exited = false;
                }
                *cooldown = 40;
            }
            else
            {
                allGhosts->ghosts[i].rect.x = allGhosts->ghosts[i].respawnPoint.x;
                allGhosts->ghosts[i].rect.y = allGhosts->ghosts[i].respawnPoint.y;
                allGhosts->ghosts[i].deathTimer = 300;
                allGhosts->ghosts[i].exited = false;
                for (int k = 0; k < allGhosts->ghostsNum; k++)
                {
                    if (allGhosts->ghosts[k].deathTimer > 0)
                        pacman->score += 200;
                }
            }
        }
    }
}



void all_ghosts_free(AllGhosts* allGhosts)
{
    for (int i = 0; i < allGhosts->ghostsNum; i++)
    {
        SDL_FreeSurface(allGhosts->ghosts[i].surface);
    }
    SDL_FreeSurface(allGhosts->ghostFeared);
    free(allGhosts->ghosts);
}
