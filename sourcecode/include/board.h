#pragma once
#include <stdbool.h>
#include "..\include\grid.h"
#include "..\include\list.h"
#include "..\include\player.h"

// Represents a board
typedef struct
{
	grid_t* grid;
	int pointer_x;
	int pointer_y;
	int win_cond;
	player_t* player_1;
	player_t* player_2;
	player_t* current_player;
	list_2_t* undo;
	list_2_t* redo;

} board_t;

board_t* board_new(int, int, int, player_t*, player_t*);
void board_free(board_t*);
void board_draw(board_t*, bool);
void board_play(board_t*);
void board_move_pointer(board_t*, int, int);

