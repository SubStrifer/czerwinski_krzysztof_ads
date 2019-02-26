#include "..\include\player.h"
#include <string.h>

// Allocates and returns a new player
player_t* player_new(char* name, char piece, bool ai)
{
	player_t* player = (player_t*)malloc(sizeof(player_t));
	
	strcpy(player->name, name);
	player->piece = piece;
	player->ai = ai;

	return player;
}

// Removes a player from memory
void player_free(player_t* player)
{
	free(player);
}