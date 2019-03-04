#include <stdio.h>
#include <string.h>
#include "..\include\submenu.h"

// Allocates and returns a new option
option_t* option_new(char* text, char* info)
{
	option_t* option = (option_t*)malloc(sizeof(option_t));
	strcpy(option->text, text);
	strcpy(option->info, info);
	option->fun_ptr = NULL;
	strncpy(option->args, "", 31);
	option->prev = NULL;
	option->next = NULL;

	return option;
}

// Removes an option from memory
void option_free(option_t* option)
{
	if(option != NULL)
		free(option);
}

// Allocates and returns a new submenu
submenu_t* submenu_new(char* title)
{
	submenu_t* submenu = (submenu_t*)malloc(sizeof(submenu_t));
	submenu->first = NULL;
	submenu->last = NULL;
	submenu->count = 0;
    submenu->pointer = 0;
    strcpy(submenu->title, title);

	return submenu;
}

// Removes a submenu from memory
void submenu_free(submenu_t* submenu)
{
	submenu_clear(submenu);

	free(submenu);
}

// Returns an option at index
option_t* submenu_option(submenu_t* submenu, int index)
{
	if(submenu->count == 0 || index >= submenu->count)
        return NULL;
    
    option_t* option = submenu->first;
    int pointer = 0;

    if(pointer == index)
        return option;

    while(option->next != NULL)
    {      
        option = option->next;
        pointer++;
        if(pointer == index)
            return option;
    }
    
    return NULL;
}

// Adds an option at the end of a submenu
void submenu_add(submenu_t* submenu, option_t* option)
{
	// If there is no options yet
    if(submenu->count == 0)
    {
        submenu->first = option;
        submenu->last = option;
        submenu->count = 1;
    }
    else
    {
        option_t* o = submenu->last;
        o->next = option;
        option->prev = o;
        submenu->last = option;
        submenu->count++;
    }
}

// Removes an option from a submenu
void submenu_remove(submenu_t* submenu, option_t* option)
{
	if(submenu->count == 0)
        return;

    option_t* o = submenu->first;

    // If vector was found at index 0
    if(o == option)
    {
        option_free(option);
        submenu->count--;

        if(submenu->count > 1)
        {
            o = o->next;
            o->prev = NULL;
            submenu->first = o;
        }
        return;
    }
    while(o->next != NULL)
    {
        // In middle
        if(o == option)
        {
            option_t* prev = o->prev;
            option_t* next = o->next;

            next->prev = prev;
            prev->next = next;

            option_free(option);
            submenu->count--;
            return;
        }
        o = o->next;
    }
    // At the end
    if(o == option)
    {
        o = o->prev;
        o->next = NULL;
        submenu->last = o;
       	option_free(option);
        submenu->count--;
        return;
    }
}

// Removes all elements from a submenu and frees memory
void submenu_clear(submenu_t* submenu)
{
	if(submenu->count == 0)
        return;

    option_t* o = submenu->first;

    while(o->next != NULL)
    {
        option_t* remove = o;
        option_free(remove);
        o = o->next;
    }
    option_free(o);

    submenu->first = NULL;
    submenu->last = NULL;
    submenu->count = 0;
}