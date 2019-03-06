#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\include\menu.h"
#include "..\include\game.h"
#include "..\include\input.h"
#include "..\include\file.h"
#include "..\include\functions.h"

submenu_t* main_menu;
submenu_t* replays_menu;
submenu_t* settings_menu;

menu_option(submenu_t*, char*, char*, void(*fun_ptr)(char*), char*);

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
	menu_option(main_menu, "Human vs Human", "Classic duel", fun_play, "0");
	menu_option(main_menu, "Human vs AI", "Face relentless AI", fun_play, "1");
	menu_option(main_menu, "AI vs Human", "Face relentless AI", fun_play, "2");
	menu_option(main_menu, "AI vs AI", "Watch AI struggle with itself", fun_play, "3");
	menu_option(main_menu, "Replays", "Replay past games", fun_replays, "");
	menu_option(main_menu, "Settings", "Adjust gameplay", fun_settings, "");
	menu_option(main_menu, "How to play", "Instructions about gameplay", fun_how_to, "");
	submenu_add(main_menu, option_new("Exit", "Close the game"));

	// Settings
	settings_menu = submenu_new("Settings");
	submenu_add(settings_menu, option_new("Back", "Go back to main menu"));
	menu_option(settings_menu, "Player 1 name", "Change Player 1 name", fun_name, "1");
	menu_option(settings_menu, "Player 2 name", "Change Player 2 name", fun_name, "2");
	menu_option(settings_menu, "Player 1 piece", "Change Player 1 piece", fun_piece, "1");
	menu_option(settings_menu, "Player 2 piece", "Change Player 2 piece", fun_piece, "2");
	menu_option(settings_menu, "Board width", "Change Board width", fun_board_w, "");
	menu_option(settings_menu, "Board height", "Change Board height", fun_board_h, "");
	menu_option(settings_menu, "Undo and Redo", "Enable or Disable Undo and Redo", fun_setting, "7");
	menu_option(settings_menu, "Draw AI weights", "Draw weights before every move", fun_setting, "8");
	
	// Update Settings texts
	sprintf(submenu_option(settings_menu, 3)->text, "Player 1 piece: %c", game_settings->player_1_piece);
	sprintf(submenu_option(settings_menu, 4)->text, "Player 2 piece: %c", game_settings->player_2_piece);

	sprintf(submenu_option(settings_menu, 5)->text, "Board width: < %d >", game_settings->board_width);
	sprintf(submenu_option(settings_menu, 6)->text, "Board height: < %d >", game_settings->board_height);

	if(game_settings->undo_redo)
		strcpy(submenu_option(settings_menu, 7)->text, "Undo and Redo: yes");
	else
		strcpy(submenu_option(settings_menu, 7)->text, "Undo and Redo: no");

	if(game_settings->draw_weights)
		strcpy(submenu_option(settings_menu, 8)->text, "Draw AI weights: yes");
	else
		strcpy(submenu_option(settings_menu, 8)->text, "Draw AI weights: no");
	
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
		case KEY_LEFT:
			// Execute a command with "-1" args
			if(highlighted->fun_ptr != NULL)
				highlighted->fun_ptr("-1");
			break;
		case KEY_RIGHT:
			// Execute a command with "1" args
			if(highlighted->fun_ptr != NULL)
				highlighted->fun_ptr("1");
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



