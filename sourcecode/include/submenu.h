#pragma once
#include <stdbool.h>

// One option in menu
typedef struct option
{
	char text[32];// Option text
	char info[32];// More informations about option
    void (*fun_ptr)(char*);// Pointer to function, NULL will act as a go back option
    char args[32];// Arguments for the function
    struct option* prev;
    struct option* next;

} option_t;

// Submenu
typedef struct
{
    option_t* first;
    option_t* last;
    int count;
    int pointer;// Highlited option, -1 will return to previous submenu
    char title[32];

} submenu_t;

option_t* option_new(char*, char*);
void option_free(option_t*);

submenu_t* submenu_new(char*);
void submenu_free(submenu_t*);
option_t* submenu_option(submenu_t*, int);
void submenu_add(submenu_t*, option_t*);
void submenu_remove(submenu_t*, option_t*);
void submenu_clear(submenu_t*);