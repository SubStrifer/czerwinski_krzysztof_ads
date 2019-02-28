#include <stdio.h>
#include "..\include\vector.h"

// Allocates and returns a new vector_2
vector_2_t* vector_2_new(int x, int y)
{
    vector_2_t* vector_2 = (vector_2_t*)malloc(sizeof(vector_2_t));
    vector_2->x = x;
    vector_2->y = y;
    vector_2->prev = NULL;
    vector_2->next = NULL;

    return vector_2;
}

// Removes a vector_2 from memory
void vector_2_free(vector_2_t* vector_2)
{
    free(vector_2);
}