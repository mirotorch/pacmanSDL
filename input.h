#ifndef INPUT
#define INPUT


typedef struct InputField
{
    int** intField;
    int rows;
    int cols;
} InputField;

void read_input(char* path, InputField* field);
void free_input_field(InputField* field);

#endif