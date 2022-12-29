#include "field.h"

void field_init(InputField* input, Field* field, int windowWidth, int windowHeight, int blockSize)
{
    field->size.x = input->cols;
    field->size.y = input->rows;
    field->blockSize = blockSize;
    srand(time(NULL));
    field->randTimer = rand() % 10000;
    int width = input->cols * blockSize;
    int height = input->rows * blockSize;
    field->beginning.x = (windowWidth - width) / 2;
    field->beginning.y = (windowHeight - height) / 2;
    field->blocks = malloc(sizeof(Block*) * input->rows);
    for(int i = 0; i < input->rows; i++)
    {
        field->blocks[i] = malloc(sizeof(Block) * input->cols);
    }

    for(int i = 0; i < input->rows; i++)
    {
        for(int k = 0; k < input->cols; k++)
        {
            field->blocks[i][k].type = input->intField[i][k];
            if(input->intField[i][k] == 0 || input->intField[i][k] == 2)
                field->blocks[i][k].collected = false;
            else
                field->blocks[i][k].collected = true;

            field->blocks[i][k].rectangle.h = blockSize;
            field->blocks[i][k].rectangle.w = blockSize;
            field->blocks[i][k].rectangle.x = field->beginning.x + blockSize * k;
            field->blocks[i][k].rectangle.y = field->beginning.y + blockSize * i;
            if(field->blocks[i][k].type == 4) field->exit = &field->blocks[i][k].rectangle;
        }
    }
}

void count_scorepoints(Field* field)
{
    field->scorepointsLeft = 0;
    for(int i = 0; i < field->size.y; i++)
    {
        for(int k = 0; k < field->size.x; k++)
        {
            if(field->blocks[i][k].collected == false) 
                field->scorepointsLeft++;
        }
    }
}


bool listcmpr(int* ls1, int* ls2, int size)
{
    bool response = true;
    for(int i = 0; i < size; i++)
    {
        if(ls1[i] != ls2[i]) response = false;
    }
    return response;
}

int get_wall_type(int y, int x, SDL_Point size, Block** blocks)
{
    int neighbours[3][3];
    int a = 0; int b = 0;
    for(int i = y - 1; i <= y + 1; i++)
    {
        for (int k = x - 1; k <= x + 1; k++)
        {
            if (i < 0 || k < 0 || i >= size.y || k >= size.x)
                neighbours[a][b] = 0;
            else if (blocks[i][k].type != 1)
            {
                neighbours[a][b] = 0;
            }
            else
            {
                neighbours[a][b] = 1;
            }
            b++;
        }
        a++;
        b = 0;
    }

    int value[4] = {neighbours[0][1], neighbours[1][0], neighbours[1][2], neighbours[2][1]};

    int cmpr[4] = {0, 0, 0, 0};
    if(listcmpr(value, cmpr, 4)) return 0;

    cmpr[2] = 1; cmpr[1] = 1;
    if(listcmpr(value, cmpr, 4)) return 1;

    cmpr[2] = 0; cmpr[1] = 0; cmpr[0] = 1; cmpr[3] = 1;
    if(listcmpr(value, cmpr, 4)) return 2;    
   

    cmpr[0] = 0; cmpr[3] = 1;
    if(listcmpr(value, cmpr, 4)) return 3;

    cmpr[0] = 1; cmpr[3] = 0;
    if(listcmpr(value, cmpr, 4)) return 4;

    cmpr[0] = 0; cmpr[1] = 1; cmpr[3] = 0;
    if(listcmpr(value, cmpr, 4)) return 5;

    cmpr[1] = 0; cmpr[2] = 1;
    if(listcmpr(value, cmpr, 4)) return 6;    

    return 7;
}


void load_textures(Field* field, SDL_Renderer* ren)
{
    for (int i = 0; i < field->size.y; i++)
    {
        for (int k = 0; k < field->size.x; k++)
        {
            switch (field->blocks[i][k].type)
            {

            case 0:
                SDL_Surface* pointSurface = SDL_LoadBMP("src/point.bmp");
                field->blocks[i][k].texture = SDL_CreateTextureFromSurface(ren, pointSurface);
                SDL_FreeSurface(pointSurface);
                break;

            case 1:
                int wallType = get_wall_type(i, k, field->size, field->blocks);
                char intStr[10];
                sprintf(intStr, "wall%d.bmp", wallType);
                char tmp[30] = "src/";
                strcat(tmp, intStr);
                SDL_Surface* wallSurface = SDL_LoadBMP(tmp);
                field->blocks[i][k].texture = SDL_CreateTextureFromSurface(ren, wallSurface);
                SDL_FreeSurface(wallSurface);
                break;

            case 2:
                SDL_Surface* tabletSurface = SDL_LoadBMP("src/tablet.bmp");
                field->blocks[i][k].texture = SDL_CreateTextureFromSurface(ren, tabletSurface);
                SDL_FreeSurface(tabletSurface);
                break;

            case 4:
                int gateType = get_gate_type(i, k, field->size, field->blocks);
                char intStr2[10];
                sprintf(intStr2, "gate%d.bmp", gateType);
                char tmp2[30] = "src/";
                strcat(tmp2, intStr2);
                SDL_Surface* gateSurface = SDL_LoadBMP(tmp2);
                field->blocks[i][k].texture = SDL_CreateTextureFromSurface(ren, gateSurface);
                SDL_FreeSurface(gateSurface);
                break;
            }
        }
    }
}

int get_gate_type(int y, int x, SDL_Point size, Block** blocks)
{
    int neighbours[3][3];
    int a = 0; int b = 0;
    for(int i = y - 1; i <= y + 1; i++)
    {
        for (int k = x - 1; k <= x + 1; k++)
        {
            if (i < 0 || k < 0 || i >= size.y || k >= size.x)
                neighbours[a][b] = 0;
            else if (blocks[i][k].type != 1)
            {
                neighbours[a][b] = 0;
            }
            else
            {
                neighbours[a][b] = 1;
            }
            b++;
        }
        a++;
        b = 0;
    }
    int value[4] = {neighbours[0][1], neighbours[1][0], neighbours[1][2], neighbours[2][1]};

    int cmpr[4] = {1, 0, 0, 1};
    if(listcmpr(value, cmpr, 4)) return 0;  

    cmpr[0] = 0; cmpr[1] = 1; cmpr[2] = 1; cmpr[3] = 0;
    if(listcmpr(value, cmpr, 4)) return 1;  

    puts("Input error: wrong ghost's gate");
    exit(-1);
}

SDL_Point get_start_position(Field* field)
{
    SDL_Point result;
    for (int i = 0; i < field->size.y; i++)
    {
        for (int k = 0; k < field->size.x; k++)
        {
            if(field->blocks[i][k].type == 5)
            {
                result.x = field->blocks[i][k].rectangle.x;
                result.y = field->blocks[i][k].rectangle.y;
                break;
            }
        }
    }
    return result;
}

void draw_field(SDL_Renderer *ren, Field *field)
{
    for (int i = 0; i < field->size.y; i++)
    {
        for (int k = 0; k < field->size.x; k++)
        {
           if ((field->blocks[i][k].type == 0 || field->blocks[i][k].type == 2) && field->blocks[i][k].collected == false
              || field->blocks[i][k].type != 0 && field->blocks[i][k].type != 2)
           {
                SDL_RenderCopy(ren, field->blocks[i][k].texture, NULL, &field->blocks[i][k].rectangle);
           }
        }
    }
}

void free_field(Field* field)
{
    for(int i = 0; i < field->size.y; i++)
    {
        free(field->blocks[i]);
    }
    free(field->blocks);
}
