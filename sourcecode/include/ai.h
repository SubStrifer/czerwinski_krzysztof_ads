#pragma once
#include "..\include\grid.h"

// Represents an ai controller
typedef struct
{
	grid_t* weights;
	grid_t* weights_temp;
	int pointer_x;
	int pointer_y;
} ai_t;

ai_t* ai_new();
void ai_free(ai_t*);
void ai_calculate(ai_t*);