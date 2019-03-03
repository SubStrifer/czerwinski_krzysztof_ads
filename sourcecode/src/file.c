#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "..\include\file.h"

// Saves a replay to a file
bool replay_save(char* file, board_t* board)
{
    FILE* outfile;
    
    // Open file
    outfile = fopen(file, "w");

    if (outfile == NULL) 
    { 
        fprintf(stderr, "\n# Error opening a file.\n"); 
        return false;
    }
    
    bool* t = (bool*)malloc(sizeof(bool));
    bool* f = (bool*)malloc(sizeof(bool));

    // Saving board info
    fwrite(&board->grid->width, sizeof(int), 1, outfile);
    fwrite(&board->grid->height, sizeof(int), 1, outfile);
    fwrite(&board->win_cond, sizeof(int), 1, outfile);

    // Saving players info
    fwrite(&board->player_1->name, sizeof(char) * 20, 1, outfile);
    fwrite(&board->player_1->piece, sizeof(char), 1, outfile);
    if(board->player_1->ai != NULL)
        fwrite(&t, sizeof(bool), 1, outfile);
    else
        fwrite(&f, sizeof(bool), 1, outfile);

    fwrite(&board->player_2->name, sizeof(char) * 20, 1, outfile);
    fwrite(&board->player_2->piece, sizeof(char), 1, outfile);
    if(board->player_2->ai != NULL)
        fwrite(&t, sizeof(bool), 1, outfile);
    else
        fwrite(&f, sizeof(bool), 1, outfile);

    free(t);
    free(f);
    // Saving moves
    vector_2_t* move = board->undo->last;

    while(move != NULL)
    {
        fwrite(&move->x, sizeof(int), 1, outfile);
        fwrite(&move->y, sizeof(int), 1, outfile);
        move = move->prev;
    }

    //if(fwrite != 0)
  
    // Close file 
    fclose (outfile);

    return true;
}

// Loads a replay from a file and returns ready to replay board
board_t* replay_load(char* file)
{
    // Preparing pointers
    board_t* board;
    player_t* player_1;
    player_t* player_2;

    FILE* infile;

    // Open file
    infile = fopen(file, "r");

    if (infile == NULL)
    { 
        fprintf(stderr, "\nError opening a file\n");
        return NULL;
    }

    // Reading board info
    int* w = (int*)malloc(sizeof(int));
    int* h = (int*)malloc(sizeof(int));;
    int* win = (int*)malloc(sizeof(int));;

    fread(w, sizeof(int), 1, infile);
    fread(h, sizeof(int), 1, infile);
    fread(win, sizeof(int), 1, infile);

    board = board_new(*w, *h, *win);

    free(w);
    free(h);
    free(win);
    
    // Reading players info
    char* name = (char*)malloc(sizeof(char) * 20);
    char piece;
    bool* ai = (bool*)malloc(sizeof(bool));

    fread(name, sizeof(char) * 20, 1, infile);
    fread(&piece, sizeof(char), 1, infile);
    fread(ai, sizeof(bool), 1, infile);

    player_1 = player_new(name, piece, ai);

    fread(name, sizeof(char) * 20, 1, infile);
    fread(&piece, sizeof(char), 1, infile);
    fread(ai, sizeof(bool), 1, infile);

    player_2 = player_new(name, piece, ai);

    free(name);
    free(ai);

    // Reading moves
    int* x = (int*)malloc(sizeof(int));
    int* y = (int*)malloc(sizeof(int));

    while(fread(x, sizeof(int), 1, infile))
    {
        fread(y, sizeof(int), 1, infile);
        list_2_add(board->redo, vector_2_new(*x, *y));
    }

    free(x);
    free(y);

    // Close file 
    fclose (infile);

    board->player_1 = player_1;
    board->player_2 = player_2;
    board->current_player = player_1;

    return board;
}