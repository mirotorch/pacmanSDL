#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_input(char* path, InputField* field)
{
    FILE *input = NULL;
    input = fopen(path, "r");
    if(input == 0)
    {
        puts("Could not open the file\n");
        exit(-1);
    }
    int rowsCount = 0;
    for (char c = getc(input); c != EOF; c = getc(input))
        if (c == '\n')
            rowsCount++;
    if (rowsCount == 0)
    {
        puts("Wrong input\n");
        exit(-1);
    }
    rowsCount++;
    fseek(input, 0, SEEK_SET);
    field->rows = rowsCount;
    int colsCount = 0;
    char tmp[255];
    for(int i = 0; i < rowsCount; i++)
    {
        fgets(tmp, 255, input);
        if(strlen(tmp) > colsCount)
            colsCount = strlen(tmp);
    }
    field->cols = colsCount - 1;
    field->intField = malloc(sizeof(int*) * field->rows);
    for(int i = 0; i < field->rows; i++)
    {
        field->intField[i] = malloc(sizeof(int) * field->cols);
    }
    fseek(input, 0, SEEK_SET);


    for(int i = 0; i < rowsCount; i++)
    {
        fgets(tmp, 255, input);
        int len = strlen(tmp) - 1;
        if(i == rowsCount - 1) len++;
        for(int k = 0; k < len; k++)
        {
            int c = tmp[k] - 48;
            field->intField[i][k] = tmp[k] - 48;
        }
        if(strlen(tmp) < field->cols)
        {
            for(int k = strlen(tmp) - 1; k < field->cols; k++)
            {
                field->intField[i][k] = 1;
            }
        }
    }
    fclose(input);
}

void free_input_field(InputField* field)
{
    for (int i = 0; i < field->rows; i++)
    {
        free(field->intField[i]);
    }
    free(field->intField);
}