#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "..\include\game.h"
#include "..\include\file.h"

const char* GAME_VERSION = "0.12";
const int GAME_WIDTH = 40;

board_t* game_board;
settings_t* game_settings;

// Main
int main(int argc, char **argv)
{
	game_settings = settings_new();
	settings_load(game_settings, "settings");
	
	menu();

	settings_free(game_settings);

	return 0;
}
