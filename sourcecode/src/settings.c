#include <stdio.h>
#include <string.h>
#include "..\include\settings.h"
#include "..\include\file.h"

// Allocates and returns a new settings_t
settings_t* settings_new()
{
    settings_t* settings = (settings_t*)malloc(sizeof(settings_t));
    // Default values
    settings->board_width = 3;
    settings->board_height = 3;
    strcpy(settings->player_1_name, "Player 1");
    settings->player_1_piece = 'X';
    strcpy(settings->player_2_name, "Player 2");
    settings->player_2_piece = 'O';
    settings->replay_last = -1;
    settings->undo_redo = true;
    settings->draw_weights = false;

    return settings;
}

// Removes a settings_t from memory
void settings_free(settings_t* settings)
{
    if(settings != NULL)
        free(settings);
}

// Loads settings from a file
void settings_load(settings_t* settings, char* file)
{
    FILE* infile;

    // Open file
    infile = fopen(file, "r");

    // If there's no settings file, try to save settings
    if (infile == NULL)
    { 
        settings_save(settings, file);
        return;
    }

    fread(settings, sizeof(settings_t), 1, infile);

    // Close file 
    fclose (infile);
}

// Saves settings to a file
bool settings_save(settings_t* settings, char* file)
{
    FILE* outfile;

    // Open file
    outfile = fopen(file, "w");

    if (outfile == NULL) 
    { 
        fprintf(stderr, "\n# Error opening a file.\n"); 
        return false;
    }

    fwrite(settings, sizeof(settings_t), 1, outfile);

    // Close file 
    fclose (outfile);

    return true;
}

// Returns next available filename for a new replay file
char* settings_new_replay(settings_t* settings)
{
    int number = -1;
    char* file = (char*)malloc(sizeof(char) * 32);
    strncpy(file, "", 31);

    // Checking files
    do
    {
        number++;
        strncpy(file, "", 31);
        sprintf(file, "%d", number);
        strcat(file, ".replay\0");
    }
    while(file_exists(file));

    // Changing the last_replay number if it's bigger than the current one
    if(number > settings->replay_last)
    {
        settings->replay_last = number;
        settings_save(settings, "settings");
    }

    return file;
}