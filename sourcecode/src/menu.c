#include <stdio.h>
#include <string.h>
#include "../include/menu.h"
#include "../include/game.h"

#define OPTIONS_COUNT 5

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
	print_row(22);
	printf(" Tic-Tac-Toe v%s\n", GAME_VERSION);
	printf(" Krzysztof Czerwinski\n");
	print_row(22);
}

// Displaying bottom information
void display_tail()
{
	print_row(22);
	printf(" Arrows to navigate,\n");
	printf(" enter to confirm.\n");
}

// Initializes possible options in menu
void init()
{
	strcpy(options[0].text, "Human vs Human");
	strcpy(options[1].text, "Human vs AI");
	strcpy(options[2].text, "AI vs AI");
	strcpy(options[3].text, "Settings");
	strcpy(options[4].text, "Exit");

	strcpy(options[0].info, "Classic duel.");
	strcpy(options[1].info, "Face relentless AI.");
	strcpy(options[2].info, "Watch AI struggle with itself.");
	strcpy(options[3].info, "Adjust gameplay.");
	strcpy(options[4].info, "Close the game.");
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

// Prints a row of specific length with a + at start and end
void print_row(int length)
{
	printf("+");
	for (int i = 0; i < length - 2; i++)
	{
		printf("-");
	}
	printf("+\n");
}