#pragma once
#include "..\include\vector.h"

// List structure that holds vector_2 structures
typedef struct
{
	vector_2_t* first;
	vector_2_t* last;
	int count;

} list_2_t;

// List structure that holds vector_t structures
typedef struct
{
	vector_t* first;
	vector_t* last;
	int count;

} list_t;

list_2_t* list_2_new();
void list_2_free(list_2_t*);
vector_2_t* list_2_vector(list_2_t*, int);
void list_2_add(list_2_t*, vector_2_t*);
void list_2_remove(list_2_t*, vector_2_t*);
void list_2_clear(list_2_t*);

list_t* list_new();
void list_free(list_t*);
vector_t* list_vector(list_t*, int);
void list_add(list_t*, vector_t*);
void list_remove(list_t*, vector_t*);
void list_clear(list_t*);