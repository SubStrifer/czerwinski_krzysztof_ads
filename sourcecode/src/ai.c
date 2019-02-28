#include <time.h>
#include "..\include\ai.h"
#include "..\include\game.h"
#include "..\include\list.h"

int count_process(int);
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

// Modifies count value before
int count_process(int count)
{
	if(game_board->win_cond == 3)
		return count * count;
	if(game_board->win_cond == 4)
		return count * count * count;
	// For win_cond 5
	switch (count)
	{
		case 1:
			return 1;
		case 2:
			return 5;
		case 3:
			return 25;
		case 4:
			return 200;
		default:
			return 0;
	}
	return count;
}

// Calculates the best move and sets ai pointer to it
void ai_calculate(ai_t* ai)
{
	grid_t* game_grid = game_board->grid;
	int width = game_grid->width;
	int height = game_grid->height;
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

	// Check both players
	for (int player = 1; player < 3; player++)
	{
		// Setting other player id
		int other_player;
		if (player == 1)
			other_player = 2;
		else
			other_player = 1;
		// Check each field
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int count = 0;

				// Checking horizontal
				for (int i = 0; i < game_board->win_cond; i++)
				{
					// If coordinates are out of bounds
					if (x + i >= width)
						break;
					// If in the current cell is the player that it's being checked now
					if (grid_cell(game_grid, x + i, y)->value == player)
						count += (6 - game_board->win_cond);
					// If there's other player on the way - break
					else if (grid_cell(game_grid, x + i, y)->value == other_player)
					{						
						count = 0;
						break;						
					}
				}
				// Adding weights
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x + i >= width)
						break;
					// Add weights to structures
					if(grid_cell(ai->weights_temp, x + i, y)->value < count_process(count))
						grid_cell(ai->weights_temp, x + i, y)->value = count_process(count);
				}
				count = 0;

				// Checking -horizontal
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x - i < 0) break;
					if (grid_cell(game_grid, x - i, y)->value == player)
						count += (6 - game_board->win_cond);
					else if (grid_cell(game_grid, x - i, y)->value == other_player)
					{
						count = 0;
						break;
					}
				}
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x - i < 0) break;
					if (grid_cell(ai->weights_temp, x - i, y)->value < count_process(count))
						grid_cell(ai->weights_temp, x - i, y)->value = count_process(count);
				}
				count = 0;

				// Checking vertical
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (y + i >= height) break;
					if (grid_cell(game_grid, x, y + i)->value == player)
						count += (6 - game_board->win_cond);
					else if (grid_cell(game_grid, x, y + i)->value == other_player)
					{
						count = 0;
						break;
					}
				}
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (y + i >= height) break;
					if (grid_cell(ai->weights_temp, x, y + i)->value < count_process(count))
						grid_cell(ai->weights_temp, x, y + i)->value = count_process(count);
				}
				count = 0;

				// Checking -vertical
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (y - i < 0) break;
					if (grid_cell(game_grid, x, y - i)->value == player)
						count += (6 - game_board->win_cond);
					else if (grid_cell(game_grid, x, y - i)->value == other_player)
					{
						count = 0;
						break;
					}
				}
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (y - i < 0) break;
					if (grid_cell(ai->weights_temp, x, y - i)->value < count_process(count))
						grid_cell(ai->weights_temp, x, y - i)->value = count_process(count);
				}
				count = 0;

				// Checking 1. diagonal
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x + i >= width || y + i >= height) break;
					if (grid_cell(game_grid, x + i, y + i)->value == player)
						count += (6 - game_board->win_cond);
					else if (grid_cell(game_grid, x + i, y + i)->value == other_player)
					{
						count = 0;
						break;
					}
				}
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x + i >= width || y + i >= height) break;
					if (grid_cell(ai->weights_temp, x + i, y + i)->value < count_process(count))
						grid_cell(ai->weights_temp, x + i, y + i)->value = count_process(count);
				}
				count = 0;

				// Checking 2.  diagonal
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x - i < 0 || y + i >= height) break;
					if (grid_cell(game_grid, x - i, y + i)->value == player)
						count += (6 - game_board->win_cond);
					else if (grid_cell(game_grid, x - i, y + i)->value == other_player)
					{
						count = 0;
						break;
					}
				}
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x - i < 0 || y + i >= height) break;
					if (grid_cell(ai->weights_temp, x - i, y + i)->value < count_process(count))
						grid_cell(ai->weights_temp, x - i, y + i)->value = count_process(count);
				}
				count = 0;

				// Checking 3. diagonal
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x + i >= width || y - i < 0) break;
					if (grid_cell(game_grid, x + i, y - i)->value == player)
						count += (6 - game_board->win_cond);
					else if (grid_cell(game_grid, x + i, y - i)->value == other_player)
					{
						count = 0;
						break;
					}
				}
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x + i >= width || y - i < 0) break;
					if (grid_cell(ai->weights_temp, x + i, y - i)->value < count_process(count))
						grid_cell(ai->weights_temp, x + i, y - i)->value = count_process(count);
				}
				count = 0;

				// Checking 4. diagonal
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x - i < 0 || y - i < 0) break;
					if (grid_cell(game_grid, x - i, y - i)->value == player)
						count += (6 - game_board->win_cond);
					else if (grid_cell(game_grid, x - i, y - i)->value == other_player)
					{
						count = 0;
						break;
					}
				}
				for (int i = 0; i < game_board->win_cond; i++)
				{
					if (x - i < 0 || y - i < 0) break;
					if (grid_cell(ai->weights_temp, x - i, y - i)->value < count_process(count))
						grid_cell(ai->weights_temp, x - i, y - i)->value = count_process(count);
				}
				count = 0;
			}
		}

	}

	// Processing neighbours
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int multiplier = 1;

			// Setting multiplier for weights
			if(grid_cell(game_grid, x, y)->value == ai_id)
				multiplier = 1;// prevent to lose weight
			else if (grid_cell(game_grid, x, y)->value == enemy_id)
				multiplier = 2;// strive to win weight
			else
				continue;
			
			for (int yy = 0; yy < 3; yy++)
			{
				for (int xx = 0; xx < 3; xx++)
				{
					int coord_x = xx + x - 1;
					int coord_y = yy + y - 1;

					if (coord_x >= 0 && coord_y >= 0 &&
						coord_x < ai->weights->width && coord_y < ai->weights->height)
						grid_cell(ai->weights, coord_x, coord_y)->value +=
							grid_cell(ai->weights_temp, coord_x, coord_y)->value * multiplier;
				}
			}

		}
	}

	// Display weights
	//ai_weights(ai);

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
			}

		}
	}

	// Adding all cells with the best weight
	list_2_t* cells = list_2_new();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if(grid_cell(game_board->grid, x, y)->value == 0 && grid_cell(ai->weights, x, y)->value == best_weight)
				list_2_add(cells, vector_2_new(x, y));
		}
	}

	// Randomizing a cell
	time_t t;
	srand((unsigned)time(&t));

	int cell = rand() % cells->count;

	ai->pointer_x = list_2_vector(cells, cell)->x;
	ai->pointer_y = list_2_vector(cells, cell)->y;

	list_2_free(cells);
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