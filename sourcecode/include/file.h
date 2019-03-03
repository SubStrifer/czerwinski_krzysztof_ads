#pragma once
#include <stdbool.h>
#include "..\include\board.h"

bool replay_save(char*, board_t*);
board_t* replay_load(char*);