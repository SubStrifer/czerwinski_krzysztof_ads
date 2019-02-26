#pragma once

// Represents one cell in a grid
typedef struct
{
	int value;

} cell_t;

// Grid structure
typedef struct
{
	cell_t* cells;
	int width;
	int height;

} grid_t;

grid_t* grid_new(int, int);
void grid_free(grid_t*);
cell_t* grid_cell(grid_t*, int, int);