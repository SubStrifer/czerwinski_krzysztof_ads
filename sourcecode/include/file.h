#pragma once
#include <stdbool.h>
#include "..\include\board.h"

bool replay_save(board_t*);
board_t* replay_load(char*);
list_t* replay_list(int);
const char* get_file_ext(char*);
bool file_exists(char*);