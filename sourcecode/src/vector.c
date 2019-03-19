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

// Allocates and returns a new vector
vector_t* vector_new(int value)
{
    vector_t* vector = (vector_t*)malloc(sizeof(vector_t));
    vector->value = value;
    vector->prev = NULL;
    vector->next = NULL;

    return vector;
}

// Removes a vector from memory
void vector_free(vector_t* vector)
{
    free(vector);
}