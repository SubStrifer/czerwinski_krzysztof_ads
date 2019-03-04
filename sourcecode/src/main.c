#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "..\include\game.h"
#include "..\include\file.h"

const char* GAME_VERSION = "0.9";
const int GAME_WIDTH = 30;

board_t* game_board;
settings_t* game_settings;

// Main
int main(int argc, char **argv)
{
	game_settings = settings_new();
	settings_load(game_settings, "settings");
	
	int chosen = -1;

	while(chosen != 7)
	{
		chosen = menu();
		game_board = board_new(3, 3, 3);
		player_t* player_1 = player_new("Player 1", 'X', false);
		player_t* player_2 = player_new("Player 2", 'O', false);
		game_board->player_1 = player_1;
		game_board->player_2 = player_2;
		game_board->current_player = player_1;

		// Switching chosen option
		switch (chosen)
		{
		case 0:// Human vs Human
			board_play(game_board);
			printf("# Game ended.");
			replay_save(game_board);
			break;
		case 1:// Human vs AI
			// Swapping second player
			player_2 = player_new("AI", 'O', true);
			game_board->player_2 = player_2;
			board_play(game_board);
			printf("# Game ended.");
			replay_save(game_board);
			break;
		case 2:// AI vs Human
			// Swapping first player
			player_1 = player_new("AI", 'X', true);
			game_board->player_1 = player_1;
			board_play(game_board);
			printf("# Game ended.");
			replay_save(game_board);
			break;
		case 3:// AI vs AI
			// Swapping both player
			player_1 = player_new("AI 1", 'X', true);
			game_board->player_1 = player_1;
			game_board->current_player = player_1;
			player_2 = player_new("AI 2", 'O', true);
			game_board->player_2 = player_2;
			board_play(game_board);
			printf("# Game ended.");
			replay_save(game_board);
			break;
		case 4:// Replays
			menu_replays();
			break;
		case 5:// Settings
			
			break;
		case 6:// How to play
			
			break;
		default:
			printf("# See you next time!");
			break;
		}

		board_free(game_board);
		player_free(player_1);
		player_free(player_2);
	}

	return 0;
}
