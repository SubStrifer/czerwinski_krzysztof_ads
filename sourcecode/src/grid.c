#include <stdio.h>
#include "../include/grid.h"

// Allocates and returns a new grid
grid_t* grid_new(int width, int height)
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

// Removes a grid from memory
void grid_free(grid_t* grid)
{
	free(grid->cells);
	free(grid);
}

// Returns a pointer to a specific cell in a grid
cell_t* grid_cell(grid_t* grid, int x, int y)
{
	// Returning NULL if coords are incorrect
	if (x >= grid->width || y >= grid->height || x < 0 || y < 0)
		return NULL;

	return &grid->cells[y * grid->width + x];
}