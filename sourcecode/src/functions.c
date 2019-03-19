#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\include\menu.h"
#include "..\include\game.h"
#include "..\include\input.h"
#include "..\include\file.h"
#include "..\include\functions.h"

// Play function for option pointer
void fun_play(char* args)
{
	int value = atoi(args);

	// Adjusting win condition
	int win_cond = 3;
	int w = game_settings->board_width;
	int h = game_settings->board_height;

	if(w == 4 || h == 4)
		win_cond = 4;
	else if (w > 4 && h > 4)
		win_cond = 5;

	game_board = board_new(w, h, win_cond);
	player_t* player_1 = player_new(game_settings->player_1_name, game_settings->player_1_piece, false);
	player_t* player_2 = player_new(game_settings->player_2_name, game_settings->player_2_piece, false);
	game_board->player_1 = player_1;
	game_board->player_2 = player_2;
	game_board->current_player = player_1;

	// Switching game type
	switch (value)
	{
	case 0:// Human vs Human
		board_play(game_board);
		replay_save(game_board);
		break;
	case 1:// Human vs AI
		// Swapping second player
		player_2 = player_new("AI", game_settings->player_2_piece, true);
		game_board->player_2 = player_2;
		board_play(game_board);
		replay_save(game_board);
		break;
	case 2:// AI vs Human
		// Swapping first player
		player_1 = player_new("AI", game_settings->player_1_piece, true);
		game_board->player_1 = player_1;
		game_board->current_player = player_1;
		board_play(game_board);
		replay_save(game_board);
		break;
	case 3:// AI vs AI
		// Swapping both player
		player_1 = player_new("AI 1", game_settings->player_1_piece, true);
		game_board->player_1 = player_1;
		game_board->current_player = player_1;
		player_2 = player_new("AI 2", game_settings->player_2_piece, true);
		game_board->player_2 = player_2;
		board_play(game_board);
		replay_save(game_board);
		break;
	default:
		break;
	}

	board_free(game_board);
	player_free(player_1);
	player_free(player_2);

}

// Submenu for replying past games
void fun_replays(char* args)
{
	// Replays menu
	replays_menu = submenu_new("Replays");
	submenu_add(replays_menu, option_new("Back", "Go back to main menu"));

	// Loading available replays
	list_t* replays_list = replay_list(game_settings->replay_last);

	char* string = (char*)malloc(sizeof(char) * 20);
	char* text = (char*)malloc(sizeof(char) * 32);

	// Adding options to menu
	for(int i = 0; i < replays_list->count; i++)
	{
		int number = list_vector(replays_list, i)->value;
		sprintf(string, "%d", number);
		sprintf(text, "Replay %d", number);
		menu_option(replays_menu, text, "Replay selected game", fun_replay, string);
	}

	free(string);
	free(text);

	menu_draw(replays_menu);

	list_free(replays_list);
	submenu_free(replays_menu);
}

// Replying a replay
void fun_replay(char* args)
{
	if(atoi(args) < 0)
		return;

	char* file = (char*)malloc(sizeof(char) * 32);
    strncpy(file, "", 31);

	// Building replay filename
	sprintf(file, "%d", atoi(args));
	strcat(file, ".replay\0");

	board_t* board;

	if(file_exists(file))
	{
		board = replay_load(file); 
		board_replay(board);
	}

	free(file);
	board_free(board);
}

// Submenu for adjusting game settings
void fun_settings(char* args)
{
	// Changing 
	menu_draw(settings_menu);

	// Resetting submenu pointer
	settings_menu->pointer = 0;

	// Saving settings to file
	settings_save(game_settings, "settings");
}

void fun_setting(char* args)
{
	int number = atoi(args);

	switch(number)
	{
		case 7:// Undo/Redo
			game_settings->undo_redo = !game_settings->undo_redo;
			if(game_settings->undo_redo)
				strcpy(submenu_option(settings_menu, 7)->text, "Undo and Redo: yes");
			else
				strcpy(submenu_option(settings_menu, 7)->text, "Undo and Redo: no");
			break;
		case 8:// Weights
			game_settings->draw_weights = !game_settings->draw_weights;
			if(game_settings->draw_weights)
				strcpy(submenu_option(settings_menu, 8)->text, "Draw AI weights: yes");
			else
				strcpy(submenu_option(settings_menu, 8)->text, "Draw AI weights: no");
			break;
	}
}

// Change player name
void fun_name(char* args)
{
	int player = atoi(args);
	int key = -1;
	char* name = (char*)malloc(sizeof(char) * 21);
	//strncpy(name, "", 21);
	
	if(player == 1)
		strcpy(name, game_settings->player_1_name);
	else
		strcpy(name, game_settings->player_2_name);

	int length = strlen(name);
	//name[length] = '\0';

	bool loop = true;

	while(loop)
	{
		system("cls");
		menu_divider();
		menu_print_2("Change name for Player ", args, true);
		menu_divider();
		menu_print("You can only use digits and letters.", false);
		menu_print("Allowed length is 1-20 characters.", false);
		menu_divider();
		menu_print_2("Chosen name: ", name, false);
		menu_divider();
		menu_print("Press Enter to confirm.", false);
		menu_print("Press Esc to cancel.", false);
		menu_divider();

		key = get_an_key();

		switch (key)
		{
			case KEY_ENTER:// Confirming
				if(length > 0)
					loop = false;
				break;
			case KEY_ESC:
				free(name);
				return;
			case KEY_SPACE:// Adding space
				if(length > 19)
					break;
				name[length] = ' ';
				length++;
				name[length] = '\0';
				break;
			case KEY_BACKSPACE:// Removing a character
				if(length <= 0)
					break;
				length--;
				name[length] = '\0';				
				break;
			case -1:
				break;
			default:// Adding a character
				if(length > 19)
					break;
				
				name[length] = key;
				length++;
				name[length] = '\0';
				break;
		}
	}
	// Changing name if length is higher than 0
	if(player == 1)
		strcpy(game_settings->player_1_name, name);
	else
		strcpy(game_settings->player_2_name, name);

	free(name);
}

// Change player piece
void fun_piece(char* args)
{
	int player = atoi(args);
	int key = -1;
	char* pointer = (char*)malloc(sizeof(char) * 2);
	strncpy(pointer, "", 2);

	char piece;
	if(player == 1)
		piece = game_settings->player_1_piece;
	else
		piece = game_settings->player_2_piece;

	*pointer = piece;

	bool loop = true;

	while(loop)
	{
		system("cls");
		menu_divider();
		menu_print_2("Change piece for Player ", args, true);
		menu_divider();
		menu_print("You can only use digits and letters.", false);
		menu_divider();
		menu_print_2("Chosen piece: ", pointer, false);
		menu_divider();
		menu_print("Press Enter to confirm.", false);
		menu_print("Press Esc to cancel.", false);
		menu_divider();

		key = get_an_key();

		switch (key)
		{
			case KEY_ENTER:
				loop = false;
				break;
			case KEY_ESC:
				free(pointer);
				return;
			case KEY_SPACE:
				break;
			case KEY_BACKSPACE:
				break;
			case -1:
				break;
			default:
				piece = key;
				*pointer = piece;
				break;
		}
	}
	// Changing piece
	if(player == 1)
		game_settings->player_1_piece = piece;
	else
		game_settings->player_2_piece = piece;

	free(pointer);

	// Update settings texts
	sprintf(submenu_option(settings_menu, 3)->text, "Player 1 piece: %c", game_settings->player_1_piece);
	sprintf(submenu_option(settings_menu, 4)->text, "Player 2 piece: %c", game_settings->player_2_piece);
}

// Change board width
void fun_board_w(char* args)
{
	int number = atoi(args);

	if(game_settings->board_width + number >= 3 && game_settings->board_width + number <= 10)
		game_settings->board_width += number;

	// Update text
	sprintf(submenu_option(settings_menu, 5)->text, "Board width: < %d >", game_settings->board_width);
}

// Change board height
void fun_board_h(char* args)
{
	int number = atoi(args);

	if(game_settings->board_height + number >= 3 && game_settings->board_height + number <= 10)
		game_settings->board_height += number;

	// Update text
	sprintf(submenu_option(settings_menu, 6)->text, "Board height: < %d >", game_settings->board_height);

}

// Submenu showing how to play the game.
void fun_how_to(char* args)
{
	// Draw mock submenu
	while(true)
	{
		// Clearing terminal
		system("cls");
		// Printing title
		menu_divider();
		menu_print("How to play", true);
		menu_divider();
		// Printing content
		menu_print("* Rules *", true);
		menu_print(" - To win place 3, 4 or 5* pieces in a", false);
		menu_print("   row", false);
		menu_print(" * (number depends on the size of ", false);
		menu_print("   the board)", false);
		menu_print(" - You can only place piece on an ", false);
		menu_print("   empty field", false);

		menu_print(" - Game is played alternately by two ", false);
		menu_print("   players", false);
		
		menu_print("* Board *", true);
		menu_print(" * - board pointer,", false);
		menu_print(" . - empty field,", false);
		menu_print(" - Any other symbol will represent ", false);
		menu_print("   players' pieces", false);
		menu_print("* Controls *", true);
		menu_print(" Arrows - move pointer", false);
		menu_print(" Enter - place your piece", false);
		menu_print(" Z/X - undo/redo", false);
		menu_print(" * You can adjust gameplay in Settings", true);
		// Printing mock back button
		menu_row();
		menu_print("> Back", false);
		menu_divider();
		// Handling input
		int key = get_nav_key();
		if(key == KEY_ENTER || key == KEY_ESC)
			break;
	}
}