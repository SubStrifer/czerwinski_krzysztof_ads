#include <stdio.h>
#include <string.h>
#include "..\include\menu.h"
#include "..\include\game.h"
#include "..\include\input.h"
#include "..\include\file.h"

#define OPTIONS_COUNT 8

int pointer;

// One option in menu
typedef struct
{
	char text[32];// Option text
	char info[32];// More informations about option
} option_t;

option_t options[OPTIONS_COUNT];

// Displaying top information
void display_head()
{
	menu_divider();
	menu_row();
	menu_print_2("Tic-Tac-Toe v", GAME_VERSION, true);
	menu_print("Krzysztof Czerwinski", true);
	menu_row();
	menu_divider();
}

// Displaying bottom information
void display_tail()
{
	menu_divider();
	printf(" Arrows to navigate,\n");
	printf(" enter to confirm.\n");
}

// Initializes possible options in menu
void init()
{
	strcpy(options[0].text, "Human vs Human");
	strcpy(options[1].text, "Human vs AI");
	strcpy(options[2].text, "AI vs Human");
	strcpy(options[3].text, "AI vs AI");
	strcpy(options[4].text, "Replays");
	strcpy(options[5].text, "Settings");
	strcpy(options[6].text, "How to play");
	strcpy(options[7].text, "Exit");

	strcpy(options[0].info, "Classic duel.");
	strcpy(options[1].info, "Face relentless AI.");
	strcpy(options[2].info, "Face relentless AI.");
	strcpy(options[3].info, "Watch AI struggle with itself.");
	strcpy(options[4].info, "Replay past games.");
	strcpy(options[5].info, "Adjust gameplay.");
	strcpy(options[6].info, "Instructions about gameplay.");
	strcpy(options[7].info, "Close the game.");
}

// Draws and handles menu
int menu()
{
	// Initialise menu
	init();
	// Menu navigation
	int chosen = -1;// Chosen menu option
	pointer = 0;// Current option highlight
	// Loop until user chooses an option
	while (chosen == -1)
	{
		// Clearing terminal
		system("cls");
		// Printing head
		display_head();
		// Printing menu options
		for (int i = 0; i < OPTIONS_COUNT; i++)
		{
			if (pointer == i)
				printf("> ");
			else
				printf("  ");
			printf("%s\n", options[i].text);
		}
		// Printing more information about highlighted option
		printf("\n %s\n", options[pointer].info);
		// Printing tail
		display_tail();
		// Handling navigation
		switch (get_nav_key())
		{
		case KEY_UP:
			if (pointer > 0)
				pointer--;
			break;
		case KEY_DOWN:
			if (pointer < OPTIONS_COUNT - 1)
				pointer++;
			break;
		case KEY_ENTER:
			chosen = pointer;
			break;
		default:
			break;
		}
	}
	return chosen;
}

void menu_replays()
{
	//todo type filename to replay

	board_replay(replay_load("replay.txt"));
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
	printf("\n# Press any key to continue...");
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