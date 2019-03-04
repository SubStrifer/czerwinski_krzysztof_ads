#include <stdio.h>
#include "..\include\list.h"

// Allocates and returns a new list_2
list_2_t* list_2_new()
{
    list_2_t* list_2 = (list_2_t*)malloc(sizeof(list_2_t));
    list_2->first = NULL;
    list_2->last = NULL;
    list_2->count = 0;

    return list_2;
}

// Removes a list and all its vectors from memory
void list_2_free(list_2_t* list_2)
{
    list_2_clear(list_2);

    free(list_2);
}

// Returns a pointer to a vector_2 at index
vector_2_t* list_2_vector(list_2_t* list_2, int index)
{
    if(list_2->count == 0 || index >= list_2->count)
        return NULL;
    
    vector_2_t* vector = list_2->first;
    int pointer = 0;

    if(pointer == index)
        return vector;

    while(vector->next != NULL)
    {      
        vector = vector->next;
        pointer++;
        if(pointer == index)
            return vector;
    }
    
    return NULL;
}

// Adds a vector_2 at the end of a list_2
void list_2_add(list_2_t* list_2, vector_2_t* vector_2)
{
    // If there is no vectors yet
    if(list_2->count == 0)
    {
        list_2->first = vector_2;
        list_2->last = vector_2;
        list_2->count = 1;
    }
    else
    {
        vector_2_t* vector = list_2->last;
        vector->next = vector_2;
        vector_2->prev = vector;
        list_2->last = vector_2;
        list_2->count++;
    }
    
}

// Removes a vector from a list
void list_2_remove(list_2_t* list_2, vector_2_t* vector_2)
{
    if(list_2->count == 0)
        return;

    vector_2_t* vector = list_2->first;

    // If vector was found at index 0
    if(vector == vector_2)
    {
        vector_2_free(vector_2);
        list_2->count--;

        if(list_2->count > 1)
        {
            vector = vector->next;
            vector->prev = NULL;
            list_2->first = vector;
        }
        return;
    }
    while(vector->next != NULL)
    {
        // In middle
        if(vector == vector_2)
        {
            vector_2_t* prev = vector->prev;
            vector_2_t* next = vector->next;

            next->prev = prev;
            prev->next = next;

            vector_2_free(vector_2);
            list_2->count--;
            return;
        }
        vector = vector->next;
    }
    // At the end
    if(vector == vector_2)
    {
        vector = vector->prev;
        vector->next = NULL;
        list_2->last = vector;
        vector_2_free(vector_2);
        list_2->count--;
        return;
    }

}

// Removes all elements from list and frees memory
void list_2_clear(list_2_t* list_2)
{
    if(list_2->count == 0)
        return;

    vector_2_t* vector = list_2->first;
    
    while(vector->next != NULL)
    {
        vector_2_t* remove = vector;
        vector_2_free(remove);
        vector = vector->next;
    }
    vector_2_free(vector);

    list_2->first = NULL;
    list_2->last = NULL;
    list_2->count = 0;
}