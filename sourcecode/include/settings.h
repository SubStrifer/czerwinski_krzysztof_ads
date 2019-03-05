#pragma once
#include <stdbool.h>

typedef struct
{
    int board_width;
    int board_height;
    char player_1_name[20];
    char player_1_piece;
    char player_2_name[20];
    char player_2_piece;
    int replay_last;
    bool undo_redo;
    bool draw_weights;

} settings_t;

settings_t* settings_new();
void settings_free(settings_t*);
void settings_load(settings_t*, char*);
bool settings_save(settings_t*, char*);
char* settings_new_replay(settings_t*);