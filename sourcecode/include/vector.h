#pragma once

// Represents two values - data structure
typedef struct vector_2
{
	int x;
    int y;
	struct vector_2* prev;
	struct vector_2* next;

} vector_2_t;

// Represents int value - data structure
typedef struct vector
{
	int value;
	struct vector* prev;
	struct vector* next;

} vector_t;

vector_2_t* vector_2_new(int, int);
void vector_2_free(vector_2_t*);

vector_t* vector_new(int);
void vector_free(vector_t*);