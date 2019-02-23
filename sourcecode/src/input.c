#include <stdio.h>
#include "..\include\input.h"

// Returns key code for navigation keys (arrows and enter) or -1 if other key was pressed
int getNavKey()
{
	// Returning arrow keys
	if (getch() == KEY_ARROW) 
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
	if (getch() == KEY_ENTER)
		return KEY_ENTER;
	// Returning -1 for everything else
	return -1;
}

// Testing - displaying key codes until Escape is pressed
void displayKeyCodes()
{
	int c = 0;
	while (c != KEY_ESC)
	{
		printf("%02X \n", c = getch());
	}
}