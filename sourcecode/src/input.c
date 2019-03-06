#include <stdio.h>
#include "..\include\input.h"

// Returns key code for navigation keys (arrows and enter) or -1 if other key was pressed
int get_nav_key()
{
	int key = getch();
	// Returning arrow keys
	if (key == KEY_ARROW)
	{
		switch (getch()) 
		{
		case KEY_UP:
			return KEY_UP;
			break;
		case KEY_DOWN:
			return KEY_DOWN;
			break;
		case KEY_LEFT:
			return KEY_LEFT;
			break;
		case KEY_RIGHT:
			return KEY_RIGHT;
			break;
		default:
			return -1;
			break;
		}
	}
	// Returning enter
	if (key == KEY_ENTER)
		return KEY_ENTER;
	// Returning escape
	if (key == KEY_ESC)
		return KEY_ESC;
	// Z & X are handled here as an exception for undo/redo feature
	// Returning Z
	if (key == KEY_Z)
		return KEY_Z;
	// Returning X
	if (key == KEY_X)
		return KEY_X;
	// Returning -1 for everything else
	return -1;
}

// Returns key code for alphanumeric characters, KEY_ENTER, KEY_ESC, KEY_SPACE, KEY_BACKSPACE or -1 for any other character
int get_an_key()
{
	int key = getch();
	// Returning -1 in the case of arrows
	if (key == KEY_ARROW)
		return -1;
	// Returning enter
	if (key == KEY_ENTER)
		return KEY_ENTER;
	// Returning escape
	if (key == KEY_ESC)
		return KEY_ESC;
	// Returning space
	if (key == KEY_SPACE)
		return KEY_SPACE;
	// Returning backspace
	if (key == KEY_BACKSPACE)
		return KEY_BACKSPACE;
	// Returning alphanumeric character
	if ((key >= 48 && key <= 57) || (key >= 65 && key <= 90) || (key >= 97 && key <= 122))
		return key;
	// Returning -1 for everything else	
	return -1;
}

// Testing - displaying key codes until Escape is pressed
void display_key_codes()
{
	int c = 0;
	while (c != KEY_ESC)
	{
		printf("%02X \n", c = getch());
	}
}