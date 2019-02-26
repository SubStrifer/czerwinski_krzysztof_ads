#include <stdio.h>
#include "..\include\board.h"
#include "..\include\input.h"
#include "..\include\ai.h"

void board_switch_player(board_t*);
bool board_place_piece(board_t*);
int board_check_win(board_t*);

// Allocates and returns a new board
board_t* board_new(int width, int height, int win_cond, player_t* player_1, player_t* player_2)
{
	board_t* board = (board_t*)malloc(sizeof(board_t));
	board->grid = grid_new(width, height);
	board->pointer_x = 0;
	board->pointer_y = 0;
	board->win_cond = win_cond;
	board->player_1 = player_1;
	board->player_2 = player_2;
	board->current_player = player_1;

	return board;
}

// Removes a board from memory
void board_free(board_t* board)
{
	grid_free(board->grid);
	free(board);
}

// Draws a board
void board_draw(board_t* board, bool display_pointer)
{
	// Creating temp values
	int width = board->grid->width;
	int height = board->grid->height;

	// Printing whole board
	print_row(width + 2);
	for (int y = 0; y < height; y++)
	{
		printf("|");
		for (int x = 0; x < width; x++)
		{
			if (board->pointer_x == x && board->pointer_y == y && display_pointer == true)
			{
				printf("*");
				continue;
			}
			switch (grid_cell(board->grid, x, y)->value)
			{
			case 0:// Empty cell
				printf(".");
				break;
			case 1:// Player 1
				printf("%c", board->player_1->piece);
				break;
			case 2:// Player 2
				printf("%c", board->player_2->piece);
				break;
			default:
				printf(" ");
				break;
			}
		}
		printf("|\n");

	}
	print_row(width + 2);
}

// Executes core gameplay
void board_play(board_t* board)
{
	// Clearing terminal
	system("cls");
	// Drawing initial board
	board_draw(board, true);
	// Drawing board information
	printf("It is now %s (%c) turn!\n", board->current_player->name, board->current_player->piece);
	int nav_key = -1;
	// Handling gameplay unless escape key is pressed
	while (nav_key != KEY_ESC)
	{
		// Warning about cell being occupied
		bool warning = false;
		// Human player
		if (board->current_player->ai == NULL)
		{
			nav_key = get_nav_key();
			// Handling navigation
			switch (nav_key)
			{
			case KEY_UP:
				board_move_pointer(board, 0, -1);
				break;
			case KEY_DOWN:
				board_move_pointer(board, 0, 1);
				break;
			case KEY_LEFT:
				board_move_pointer(board, -1, 0);
				break;
			case KEY_RIGHT:
				board_move_pointer(board, 1, 0);
				break;
			case KEY_ENTER:
				// Placing a piece
				if (board_place_piece(board))
					// Switching player if piece has been placed
					board_switch_player(board);
				else
					warning = true;
				break;
			default:
				break;
			}
		}
		else// AI player
		{
			ai_calculate(board->current_player->ai);
			board->pointer_x = board->current_player->ai->pointer_x;
			board->pointer_y = board->current_player->ai->pointer_y;
			// Placing a piece
			if (board_place_piece(board))
				// Switching player if piece has been placed
				board_switch_player(board);
			else
				warning = true;
		}
		// Clearing terminal
		system("cls");
		// Checking if somebody wins
		switch (board_check_win(board))
		{
		case 1:
			// Redrawing the board
			board_draw(board, false);
			printf("%s wins!\n", board->player_1->name);
			return;
		case 2:
			// Redrawing the board
			board_draw(board, false);
			printf("%s wins!\n", board->player_2->name);
			return;
		case 3:
			// Redrawing the board
			board_draw(board, false);
			printf("It's a tie!\n");
			return;
		default:
			// Check if the next player is AI
			if(board->current_player->ai == NULL)
			// Redrawing the board
				board_draw(board, true);
			else
				board_draw(board, false);
			break;
		}
		// Drawing board information
		printf("It is now %s (%c) turn!\n", board->current_player->name, board->current_player->piece);
		// Drawing warning
		if(warning)
			printf("This cell is already occupied!\n");
	}
}

// Tries to move a board by specific coords
void board_move_pointer(board_t* board, int x, int y)
{
	// Return if pointer moves beyond the board
	if (board->pointer_x + x >= board->grid->width ||
		board->pointer_x + x < 0 ||
		board->pointer_y + y >= board->grid->height ||
		board->pointer_y + y < 0)
		return;
	// Move the pointer
	board->pointer_x += x;
	board->pointer_y += y;
}

// Switches the current player
void board_switch_player(board_t* board)
{
	if (board->current_player == board->player_1)
		board->current_player = board->player_2;
	else
		board->current_player = board->player_1;
}

// Places a piece on the board using pointer coords
bool board_place_piece(board_t* board)
{
	// Check if a cell is empty
	if (grid_cell(board->grid, board->pointer_x, board->pointer_y)->value != 0)
		return false;

	// Placing a piece is actually just a value in a cell
	if (board->current_player == board->player_1)
	{
		//board->grid->cells[pointer_index].value = 1;
		grid_cell(board->grid, board->pointer_x, board->pointer_y)->value = 1;
		return true;
	}
	else
	{
		//board->grid->cells[pointer_index].value = 2;
		grid_cell(board->grid, board->pointer_x, board->pointer_y)->value = 2;
		return true;
	}
	return false;
}

// Checks if either of the player wins
// 0 - no winner, 1 - player_1, 2 - player_2, 3 - draw
int board_check_win(board_t* board)
{
	// Check both players
	for(int player = 1; player < 3; player++)
	{
		// Check each field
		for (int y = 0; y < board->grid->height; y++)
		{
			for (int x = 0; x < board->grid->width; x++)
			{
				// Skip last corner
				if (x == board->grid->width - 1 && y == board->grid->height - 1)
					continue;

				int consecutive = 0;			
				// If current cell has a player piece
				if (grid_cell(board->grid, x, y)->value == player)
				{
					consecutive = 1;
					// Checking horizontal
					for (int i = 1; i < board->win_cond; i++)
					{
						if (x + i >= board->grid->width)
							break;
						if (grid_cell(board->grid, x + i, y)->value == player)
							consecutive++;
						else
							break;
					}
					// Player wins
					if (consecutive == board->win_cond)
						return player;
					else
						consecutive = 1;
					// Checking vertical
					for (int i = 1; i < board->win_cond; i++)
					{
						if (y + i >= board->grid->height)
							break;
						if (grid_cell(board->grid, x, y + i)->value == player)
							consecutive++;
						else
							break;
					}
					// Player wins
					if (consecutive == board->win_cond)
						return player;
					else
						consecutive = 1;
					// Checking diagonal
					for (int i = 1; i < board->win_cond; i++)
					{
						if (x + i >= board->grid->width || y + i >= board->grid->height)
							break;
						if (grid_cell(board->grid, x + i, y + i)->value == player)
							consecutive++;
						else
							break;
					}
					// Player wins
					if (consecutive == board->win_cond)
						return player;
					else
						consecutive = 1;
					// Checking second diagonal
					for (int i = 1; i < board->win_cond; i++)
					{
						if (x - i < 0 || y + i >= board->grid->height)
							break;
						if (grid_cell(board->grid, x - i, y + i)->value == player)
							consecutive++;
						else
							break;
					}
					// Player wins
					if (consecutive == board->win_cond)
						return player;
				}

			}
		}

	}
	// Check if board board is full
	if (false)
	{
		// Return draw
		return 3;
	}
	// Return no win
	return 0;
}