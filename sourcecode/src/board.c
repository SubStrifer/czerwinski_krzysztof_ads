#include <stdio.h>
#include "..\include\board.h"
#include "..\include\input.h"
#include "..\include\ai.h"
#include "..\include\game.h"
#include "..\include\menu.h"

void board_switch_player(board_t*);
bool board_place_piece(board_t*);
int board_check_win(board_t*);
bool board_undo(board_t*);
bool board_redo(board_t*);

// Allocates and returns a new board
board_t* board_new(int width, int height, int win_cond)
{
	board_t* board = (board_t*)malloc(sizeof(board_t));
	board->grid = grid_new(width, height);
	board->pointer_x = 0;
	board->pointer_y = 0;
	board->win_cond = win_cond;
	board->player_1 = NULL;
	board->player_2 = NULL;
	board->current_player = NULL;
	board->undo = list_2_new();
	board->redo = list_2_new();

	return board;
}

// Removes a board from memory
void board_free(board_t* board)
{
	if(board == NULL)
		return;

	grid_free(board->grid);
	list_2_free(board->undo);
	list_2_free(board->redo);

	free(board);
}

// Draws a board
void board_draw(board_t* board, bool display_pointer)
{
	// Creating temp values
	int width = board->grid->width;
	int height = board->grid->height;
	int shift = (GAME_WIDTH - 2 - width) / 2;
	// Printing whole board
	menu_divider();
	for (int y = 0; y < height; y++)
	{
		printf("|");
		// Print some spaces before the board
		for(int i = 0; i < shift; i++)
			printf(" ");
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
		printf("|");
		// Print some spaces after the board
		for(int i = width + 2 + shift; i < GAME_WIDTH - 2; i++)
			printf(" ");
		printf("|\n");
	}
	menu_divider();
}

// Executes core gameplay
void board_play(board_t* board)
{
	// Clearing terminal
	system("cls");
	// Drawing initial board
	board_draw(board, true);
	// Drawing board information
	menu_print(" It's now turn of:", false);
	menu_print_2(" ", board->current_player->name, false);
	menu_divider();
	int nav_key = -1;
	// Handling gameplay unless escape key is pressed
	while (nav_key != KEY_ESC)
	{
		// Warning about cell being occupied
		bool warning = false;
		// Flag for skipping Ai movement (needed for undo/redo)
		bool ai_skip = false;
		
		if (board->current_player->ai != NULL)
		{
			menu_print(" Z/X to undo/redo.", false);
			menu_print(" Any key to continue.", false);
			menu_divider();
		}
		
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
			if (board->current_player->ai != NULL)
				break;
			// Placing a piece
			if (board_place_piece(board))
				// Switching player if piece has been placed
			{
				ai_skip = true;
				board_switch_player(board);
			}
			else
				warning = true;
			break;
		case KEY_Z:
			board_undo(board);
			ai_skip = true;
			break;
		case KEY_X:
			board_redo(board);
			ai_skip = true;
			break;
		default:
			break;
		}

		// AI player
		if (board->current_player->ai != NULL && !ai_skip)
		{
			ai_calculate(board->current_player->ai);
			board->pointer_x = board->current_player->ai->pointer_x;
			board->pointer_y = board->current_player->ai->pointer_y;
			// Placing a piece
			if (board_place_piece(board))
				// Switching player if piece has been placed
				board_switch_player(board);
		}
				
		// Clearing terminal
		system("cls");
		// Checking if somebody wins
		switch (board_check_win(board))
		{
		case 1:
			// Redrawing the board
			board_draw(board, false);
			menu_print_2(board->player_1->name, " wins!", true);
			menu_wait();
			return;
		case 2:
			// Redrawing the board
			board_draw(board, false);
			menu_print_2(board->player_2->name, " wins!", true);
			menu_wait();
			return;
		case 3:
			// Redrawing the board
			board_draw(board, false);
			menu_print("It's a tie!", true);
			menu_wait();
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
		menu_print(" It's now turn of:", false);
		menu_print_2(" ", board->current_player->name, false);
		// Drawing warning
		if(warning)
			menu_print("This cell is already occupied!", true);
		menu_divider();
	}
}

// Replays the board
void board_replay(board_t* board)
{
	// Clearing terminal
	system("cls");
	// Drawing initial board
	board_draw(board, false);
	// Drawing board information
	menu_print(" It's now turn of:", false);
	menu_print_2(" ", board->current_player->name, false);
	menu_print(" Z/X or Left/Right Arrow", false);
	menu_print(" to navigate the replay.", false);
	menu_divider();
	int nav_key = -1;
	// Handling gameplay unless escape key is pressed
	while (nav_key != KEY_ESC)
	{
		nav_key = get_nav_key();
		// Handling navigation
		switch (nav_key)
		{
		case KEY_LEFT:
			board_undo(board);
			break;
		case KEY_RIGHT:
			board_redo(board);
			break;
		case KEY_Z:
			board_undo(board);
			break;
		case KEY_X:
			board_redo(board);
			break;
		default:
			break;
		}
				
		// Clearing terminal
		system("cls");
		// Checking if somebody wins
		switch (board_check_win(board))
		{
		case 1:
			// Redrawing the board
			board_draw(board, false);
			menu_print_2(board->player_1->name, " wins!", true);
			menu_wait();
			return;
		case 2:
			// Redrawing the board
			board_draw(board, false);
			menu_print_2(board->player_2->name, " wins!", true);
			menu_wait();
			return;
		case 3:
			// Redrawing the board
			board_draw(board, false);
			menu_print("It's a tie!", true);
			menu_wait();
			return;
		default:
			board_draw(board, false);
			break;
		}

		// Drawing board information
		menu_print(" It's now turn of:", false);
		menu_print_2(" ", board->current_player->name, false);
		menu_print(" Z/X or Left/Right Arrow", false);
		menu_print(" to navigate the replay.", false);
		if(board->redo->count == 0)
		{
			menu_divider();
			menu_print(" That was the last move in this game.", false);
			menu_print(" Press Esc to exit replaying.", false);
		}
		menu_divider();
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

	int player;
	if(board->current_player == board->player_1)
		player = 1;
	else
		player = 2;

	// Placing a value that represents a piece
	grid_cell(board->grid, board->pointer_x, board->pointer_y)->value = player;
	list_2_add(board->undo, vector_2_new(board->pointer_x, board->pointer_y));
	list_2_clear(board->redo);

	return true;

}

// Removes last placed piece
bool board_undo(board_t* board)
{
	// Return if undo is empty
	if(board->undo->count == 0)
		return false;

	int x = board->undo->last->x;
	int y = board->undo->last->y;
	grid_cell(board->grid, x, y)->value = 0;// Reset piece
	list_2_add(board->redo, vector_2_new(x, y));// Add one step to redo
	list_2_remove(board->undo, board->undo->last);// Remove undo that was just used
	board_switch_player(board);// Switch player

	return true;
}

// Tries to revert undo
bool board_redo(board_t* board)
{
	// Return if redo is empty
	if(board->redo->count == 0)
		return false;

	int player;
	if(board->current_player == board->player_1)
		player = 1;
	else
		player = 2;
	
	int x = board->redo->last->x;
	int y = board->redo->last->y;

	grid_cell(board->grid, x, y)->value = player;// Placing a piece
	list_2_add(board->undo, vector_2_new(x, y));// Add one step to undo
	list_2_remove(board->redo, board->redo->last);// Remove redo that was just used
	board_switch_player(board);// Switch player

	return true;
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
	// Check if board is full
	bool full = true;

	for (int y = 0; y < board->grid->height; y++)
	{
		if(!full)
			break;

		for (int x = 0; x < board->grid->width; x++)
		{
			if(grid_cell(board->grid, x, y)->value == 0)
			{
				full = false;
				break;
			}
		}
	}
	// Return draw
	if (full)
		return 3;
	// Return no win
	return 0;
}