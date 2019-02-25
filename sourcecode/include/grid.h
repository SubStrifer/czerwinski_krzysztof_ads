#pragma once

typedef struct
{
	int value;

} cell_t;

typedef struct
{
	cell_t* cells;
	int width;
	int height;

} grid_t;

grid_t* newGrid(int, int);
cell_t* getCell(grid_t*, int, int);