#include <stdio.h>
#include "..\include\list.h"

void list_2_head(list_2_t* list_2);
void list_2_tail(list_2_t* list_2);

// Allocates and returns a new list_2
list_2_t* list_2_new()
{
    list_2_t* list_2 = (list_2_t*)malloc(sizeof(list_2_t));
    list_2->vector = NULL;
    list_2->count = 0;
    list_2->pointer = -1;

    return list_2;
}

// Removes a list and all its vectors from memory
void list_2_free(list_2_t* list_2)
{
    list_2_head(list_2);

    if(list_2->count > 0)
    {
        while(list_2->vector->next != NULL)
        {
            vector_2_t* next = list_2->vector->next;
            vector_2_free(list_2->vector);
            list_2->vector = next;
            list_2->count--;
        }
        vector_2_free(list_2->vector);
    }

    free(list_2);
}

// Returns a pointer to a vector_2 at index
vector_2_t* list_2_vector(list_2_t* list_2, int index)
{
    if(list_2->count == 0 || index >= list_2->count)
        return NULL;
    
    list_2_head(list_2);

    if(list_2->pointer == index)
            return list_2->vector;

    while(list_2->vector->next != NULL)
    {      
        list_2->vector = list_2->vector->next;
        list_2->pointer++;
        if(list_2->pointer == index)
            return list_2->vector;
    }
    
    return NULL;
}

// Adds a vector_2 at the end of a list_2
void list_2_add(list_2_t* list_2, vector_2_t* vector_2)
{
    // If there is no vectors yet
    if(list_2->count == 0)
    {
        list_2->vector = vector_2;
        list_2->count = 1;
        list_2->pointer = 0;
    }
    else
    {
        list_2_tail(list_2);
        list_2->vector->next = vector_2;
        vector_2->prev = list_2->vector;
        list_2->count++;
    }
    
}

// Removes a vector from a list
void list_2_remove(list_2_t* list_2, vector_2_t* vector_2)
{
    if(list_2->count == 0)
        return;
    
    list_2_head(list_2);
    // If vector was found at index 0
    if(list_2->vector == vector_2)
    {
        list_2->vector = list_2->vector->next;
        list_2->vector->prev = NULL;
        vector_2_free(vector_2);
        list_2->count--;
        return;
    }
    while(list_2->vector->next != NULL)
    {
        list_2->vector->next;
        list_2->pointer++;
        // In middle
        if(list_2->vector == vector_2)
        {
            vector_2_t* prev = vector_2->prev;
            vector_2_t* next = vector_2->next;

            list_2->vector = prev;
            list_2->vector->next = next;
            next->prev = prev;

            vector_2_free(vector_2);
            list_2->count--;
            list_2->pointer--;
            return;
        }
    }
    // At the end
    if(list_2->vector == vector_2)
    {
        list_2->vector = list_2->vector->prev;
        list_2->vector->next = NULL;

        vector_2_free(vector_2);
        list_2->count--;
        list_2->pointer--;
        return;
    }

}

// Moves pointer of a list to the first struct
void list_2_head(list_2_t* list_2)
{
    if(list_2->count == 0)
        return;
    
    while(list_2->vector->prev != NULL)
    {
        list_2->vector = list_2->vector->prev;
        list_2->pointer--;
    }
}

// Moves pointer of a list to the last struct
void list_2_tail(list_2_t* list_2)
{
    if(list_2->count == 0)
        return;
    
    while(list_2->vector->next != NULL)
    {
        list_2->vector = list_2->vector->next;
        list_2->pointer++;
    }
}