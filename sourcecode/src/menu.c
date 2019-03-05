#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\include\menu.h"
#include "..\include\game.h"
#include "..\include\input.h"
#include "..\include\file.h"

submenu_t* main_menu;
submenu_t* replays_menu;
submenu_t* settings_menu;

menu_option(submenu_t*, char*, char*, void(*fun_ptr)(char*), char*);
void fun_play(char*);
void fun_replays(char*);
void fun_replay(char*);
void fun_settings(char*);
void fun_how_to(char*);

// Displaying top information
void display_title(submenu_t* submenu)
{
	menu_divider();
	menu_print(submenu->title, true);
	menu_divider();
}

// Displaying bottom information
void display_footer()
{
	menu_divider();
	menu_print(" Arrows to navigate,", false);
	menu_print(" Enter to confirm.", false);
	menu_print_2("Krzysztof Czerwinski, v", GAME_VERSION, true);
	menu_divider();
}

// Initializes possible options in menu
void init()
{
	main_menu = submenu_new("Tic-Tac-Toe");

	// Main menu
	menu_option(main_menu, "Human vs Human", "Classic duel.", fun_play, "0");
	menu_option(main_menu, "Human vs AI", "Face relentless AI.", fun_play, "1");
	menu_option(main_menu, "AI vs Human", "Face relentless AI.", fun_play, "2");
	menu_option(main_menu, "AI vs AI", "Watch AI struggle with itself.", fun_play, "3");
	menu_option(main_menu, "Replays", "Replay past games.", fun_replays, "");
	menu_option(main_menu, "Settings", "Adjust gameplay.", fun_settings, "");
	menu_option(main_menu, "How to play", "Instructions about gameplay.", fun_how_to, "");
	submenu_add(main_menu, option_new("Exit", "Close the game."));

	// Settings
	settings_menu = submenu_new("Settings");
	submenu_add(settings_menu, option_new("Back", "Go back to main menu."));
}

// Draws and handles menu
void menu()
{
	// Initialise menu
	init();

	// Show main menu
	menu_draw(main_menu);

	// Free menus
	submenu_free(main_menu);
	submenu_free(settings_menu);
	
	//Printing goodbye
	system("cls");
	menu_divider();
	menu_print(" See you next time!", true);
	menu_divider();
}

// Draws and handles submenus
void menu_draw(submenu_t* submenu)
{
	// Loop until user chooses an option
	while (submenu->pointer != -1)
	{
		// Clearing terminal
		system("cls");
		// Printing head
		display_title(submenu);
		// Printing menu options
		for (int i = 0; i < submenu->count; i++)
		{
			if (submenu->pointer == i)
				menu_print_2("> ", submenu_option(submenu, i)->text, false);
			else
				menu_print_2("  ", submenu_option(submenu, i)->text, false);
		}
		option_t* highlighted = submenu_option(submenu, submenu->pointer);
		// Printing more information about highlighted option
		menu_row();
		menu_print(highlighted->info, true);
		// Printing tail
		display_footer();
		// Handling navigation
		switch (get_nav_key())
		{
		case KEY_UP:
			if (submenu->pointer > 0)
				submenu->pointer--;
			break;
		case KEY_DOWN:
			if (submenu->pointer < submenu->count - 1)
				submenu->pointer++;
			break;
		case KEY_ENTER:
			// Go back
			if(highlighted->fun_ptr == NULL)
			{
				submenu->pointer = -1;
				break;
			}
			// Execute a command
			highlighted->fun_ptr(highlighted->args);
			break;
		case KEY_ESC:
			submenu->pointer = -1;
			break;
		default:
			break;
		}
	}
	return;
}

// Prints a row with a + at start and end
void menu_divider()
{
	printf("+");
	for (int i = 0; i < GAME_WIDTH - 2; i++)
	{
		printf("-");
	}
	printf("+\n");
}

// Prints empty row with borders
void menu_row()
{
	char* text = (char*)malloc(sizeof(char) * (GAME_WIDTH + 1));
	strncpy(text, "", GAME_WIDTH);
	strcpy(text, "|");
	for(int i = 0; i < GAME_WIDTH - 2; i++)
		strcat(text, " ");
	strcat(text, "|\0");
	// Printing text
	printf("%s\n", text);
	free(text);
}

// Prints text and adds | to both sides, centers the text if needed
void menu_print(char* string, bool center)
{
	int max_length = GAME_WIDTH - 2;
	char* trun = (char*)malloc(sizeof(char) * (GAME_WIDTH + 1));
	char* text = (char*)malloc(sizeof(char) * (GAME_WIDTH + 1));
	int length = strlen(string);
	// Clear strings
	strncpy(trun, "", GAME_WIDTH);
	strncpy(text, "", GAME_WIDTH);
	// Truncating the string to max_length
	strncpy(trun, string, max_length);
	// Adding | at the beginning
	strcpy(text, "|");
	// Adding truncated string
	if(length >= max_length || !center)
	{
		strcat(text, trun);
		// Adding spaces until right border is reached
		for(int i = length; i < max_length; i++)
			strcat(text, " ");
	}
	else// If shorter than max_length and centered
	{
		int shift = (max_length - length) / 2;
		// Adding spaces before text
		for(int i = 0; i < shift; i++)
			strcat(text, " ");
		// Adding text
		strcat(text, trun);
		// Adding spaces after text
		for(int i = length + shift; i < max_length; i++)
			strcat(text, " ");
	}
	
	// Adding | at the end
	strcat(text, "|\0");
	// Printing text
	printf("%s\n", text);
	free(trun);
	free(text);
}

// Prints two strings
void menu_print_2(char* s1, char* s2, bool center)
{
	char* string = string_con(s1, s2);
	menu_print(string, center);
	free(string);
}

// Waits until user clicks a key
void menu_wait()
{
	menu_divider();
	menu_print(" Press any key to continue...", false);
	menu_divider();
	getch();
}

// Dynamically connects two string and returns the result
char* string_con(char* s1, char* s2)
{
	char* string = (char*)malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));
	strcpy(string, s1);
	strcat(string, s2);
	return string;
}

// Constructs a new submenu option and adds it to a submenu
menu_option(submenu_t* submenu, char* text, char* info, void(*fun_ptr)(char*), char* args)
{
	option_t* option = option_new(text, info);
	option->fun_ptr = fun_ptr;
	strcpy(option->args, args);
	submenu_add(submenu, option);
}

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
	submenu_add(replays_menu, option_new("Back", "Go back to main menu."));

	// Loading available replays
	list_2_t* replays_list = replay_list(game_settings->replay_last);

	char* string = (char*)malloc(sizeof(char) * 20);

	// Adding options to menu
	for(int i = 0; i < replays_list->count; i++)
	{
		int number = list_2_vector(replays_list, i)->x;
		sprintf(string, "%d", number);
		menu_option(replays_menu, string, "Replay selected game.", fun_replay, string);
	}

	free(string);

	menu_draw(replays_menu);

	list_2_free(replays_list);
	submenu_free(replays_menu);
}

// Replying a replay
void fun_replay(char* args)
{
	char* file = (char*)malloc(sizeof(char) * 32);
    strncpy(file, "", 31);

	// Building replay filename
	sprintf(file, "%d", atoi(args));
	strcat(file, ".replay\0");
	board_replay(replay_load(file));

	free(file);
}

// Submenu for adjusting game settings
void fun_settings(char* args)
{
	menu_draw(settings_menu);
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

