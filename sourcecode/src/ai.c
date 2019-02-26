#include "..\include\ai.h"
#include "..\include\game.h"

void ai_neighbours(grid_t*, grid_t*, int, int);
void ai_weights(ai_t*);

// Allocates and returns a new ai controller
ai_t* ai_new()
{
	ai_t* ai = (ai_t*)malloc(sizeof(ai_t));
	ai->weights = grid_new(game_board->grid->width, game_board->grid->height);
	ai->weights_temp = grid_new(game_board->grid->width, game_board->grid->height);
	ai->pointer_x = 0;
	ai->pointer_y = 0;
	return ai;
}

// Removes an ai from memory
void ai_free(ai_t* ai)
{
	grid_free(ai->weights);
	grid_free(ai->weights_temp);
	free(ai);
}

// Calculates the best move and sets ai pointer to it
void ai_calculate(ai_t* ai)
{
	grid_t* grid = game_board->grid;
	int width = grid->width;
	int height = grid->height;
	int ai_id = 0;
	int enemy_id = 0;
	// Setting ids
	if (ai == game_board->player_1->ai)
	{
		ai_id = 1;
		enemy_id = 2;
	}
	else
	{
		ai_id = 2;
		enemy_id = 1;
	}

	// Clearing weights
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			grid_cell(ai->weights, x, y)->value = 0;
			grid_cell(ai->weights_temp, x, y)->value = 0;
		}
	}

	// Checking every cell and adding basic weights to them
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int value = grid_cell(game_board->grid, x, y)->value;
			// Empty cell
			if (value == 0)
			{
				grid_cell(ai->weights_temp, x, y)->value += 0;
			}
			// Enemy cell
			else if (value == enemy_id)
			{
				grid_cell(ai->weights_temp, x, y)->value += 2;
			}
			//
			else if (value == ai_id)
			{
				grid_cell(ai->weights_temp, x, y)->value += 1;
			}
		}
	}

	// Adding weights to neighbours
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			ai_neighbours(ai->weights_temp, ai->weights, x, y);
		}
	}
	ai_weights(ai);
	// Iterating again and searching for the best weight
	int best_weight = -1;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int new_weight = grid_cell(ai->weights, x, y)->value;
			// Setting pointer to the new best position
			// Only when cell is empty
			if (new_weight > best_weight && grid_cell(game_board->grid, x, y)->value == 0)
			{
				best_weight = new_weight;
				ai->pointer_x = x;
				ai->pointer_y = y;
			}

		}
	}

}

// Adds own value to neighbours
void ai_neighbours(grid_t* input, grid_t* output, int x, int y)
{
	int multiplier = 1;
	for (int yy = 0; yy < 5; yy++)
	{
		for (int xx = 0; xx < 5; xx++)
		{
			// Skip some cells..
			if ((xx == 2 && yy == 2) ||
				(xx == 1 && yy == 0) ||
				(xx == 0 && yy == 1) ||
				(xx == 3 && yy == 0) || 
				(xx == 4 && yy == 1) || 
				(xx == 0 && yy == 3) || 
				(xx == 1 && yy == 4) || 
				(xx == 4 && yy == 3) || 
				(xx == 3 && yy == 4))
				continue;
			if (xx == 0 || xx == 4 || yy == 0 || yy == 4)
				multiplier = 1;
			else
				multiplier = 2;

			int coord_x = xx + x - 2;
			int coord_y = yy + y - 2;


			if (coord_x >= 0 && coord_y >= 0 &&
				coord_x < input->width && coord_y < input->height)
			grid_cell(output, coord_x, coord_y)->value += grid_cell(input, x, y)->value * multiplier;
		}
	}

}

// Prints ai weights
void ai_weights(ai_t* ai)
{
	for (int y = 0; y < ai->weights->height; y++)
	{
		printf("\n");
		for (int x = 0; x < ai->weights->width; x++)
		{
			printf("%i\t", grid_cell(ai->weights, x, y)->value);
		}
	}
	printf("\n# Press Enter to continue...");
	getchar();
}