#include <stdio.h>
#include "../include/grid.h"

grid_t* newGrid(int width, int height)
{
	grid_t* grid = (grid_t*)malloc(sizeof(grid_t));
	grid->width = width;
	grid->height = height;
	grid->cells = (cell_t*)malloc(sizeof(cell_t*) * width * height);

	// Setting all cell values to 0
	for (int i = 0; i < width * height; i++)
	{
		grid->cells[i].value = 0;
	}

	return grid;
}

cell_t* getCell(grid_t* grid, int x, int y)
{
	// Returning NULL if coords are incorrect
	if (x >= grid->width || y >= grid->height)
		return NULL;

	return &grid->cells[y * grid->width + x];
}