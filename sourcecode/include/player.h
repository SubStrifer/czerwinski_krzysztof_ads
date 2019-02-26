#pragma once
#include <stdbool.h>

typedef struct
{
	char name[20];
	char piece;
	bool ai;
} player_t;

player_t* player_new(char*, char, bool);
void player_free(player_t*);