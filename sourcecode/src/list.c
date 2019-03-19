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
        // In the middle
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

// Allocates and returns a new list
list_t* list_new()
{
    list_t* list = (list_t*)malloc(sizeof(list_t));
    list->first = NULL;
    list->last = NULL;
    list->count = 0;

    return list;
}

// Removes a list and all its vectors from memory
void list_free(list_t* list)
{
    list_clear(list);

    free(list);
}

// Returns a pointer to a vector_2 at index
vector_t* list_vector(list_t* list, int index)
{
    if(list->count == 0 || index >= list->count)
        return NULL;
    
    vector_t* vector = list->first;
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
void list_add(list_t* list, vector_t* vector)
{
    // If there is no vectors yet
    if(list->count == 0)
    {
        list->first = vector;
        list->last = vector;
        list->count = 1;
    }
    else
    {
        vector_t* temp = list->last;
        temp->next = vector;
        vector->prev = temp;
        list->last = vector;
        list->count++;
    }
    
}

// Removes a vector from a list
void list_remove(list_t* list, vector_t* vector)
{
    if(list->count == 0)
        return;

    vector_t* temp = list->first;

    // If vector was found at index 0
    if(temp == vector)
    {
        vector_free(vector);
        list->count--;

        if(list->count > 1)
        {
            temp = temp->next;
            temp->prev = NULL;
            list->first = temp;
        }
        return;
    }
    while(temp->next != NULL)
    {
        // In the middle
        if(temp == vector)
        {
            vector_t* prev = temp->prev;
            vector_t* next = temp->next;

            next->prev = prev;
            prev->next = next;

            vector_free(vector);
            list->count--;
            return;
        }
        temp = temp->next;
    }
    // At the end
    if(temp == vector)
    {
        temp = temp->prev;
        temp->next = NULL;
        list->last = temp;
        vector_free(vector);
        list->count--;
        return;
    }

}

// Removes all elements from list and frees memory
void list_clear(list_t* list)
{
    if(list->count == 0)
        return;

    vector_t* temp = list->first;
    
    while(temp->next != NULL)
    {
        vector_t* remove = temp;
        vector_free(remove);
        temp = temp->next;
    }
    vector_free(temp);

    list->first = NULL;
    list->last = NULL;
    list->count = 0;
}