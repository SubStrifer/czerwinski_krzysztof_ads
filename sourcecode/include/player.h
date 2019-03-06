#pragma once
#include <stdbool.h>
#include "..\include\ai.h"

// Represents a player
typedef struct
{
	char name[21];
	char piece;
	ai_t* ai;
} player_t;

player_t* player_new(char*, char, bool);
void player_free(player_t*);