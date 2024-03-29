#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "..\include\file.h"
#include "..\include\settings.h"
#include "..\include\game.h"
#include "..\include\list.h"

// Saves a replay to a file
bool replay_save(board_t* board)
{
    FILE* outfile;
    
    // Generating file name
    char* file = settings_new_replay(game_settings);  

    // Open file
    outfile = fopen(file, "w");

    if (outfile == NULL) 
    { 
        fprintf(stderr, "\n# Error opening a file.\n"); 
        return false;
    }

    // Saving board info
    fwrite(&board->grid->width, sizeof(int), 1, outfile);
    fwrite(&board->grid->height, sizeof(int), 1, outfile);
    fwrite(&board->win_cond, sizeof(int), 1, outfile);

    // Saving players info
    fwrite(&board->player_1->name, sizeof(char) * 21, 1, outfile);
    fwrite(&board->player_1->piece, sizeof(char), 1, outfile);

    fwrite(&board->player_2->name, sizeof(char) * 21, 1, outfile);
    fwrite(&board->player_2->piece, sizeof(char), 1, outfile);

    // Saving moves
    vector_2_t* move = board->undo->last;

    while(move != NULL)
    {
        fwrite(&move->x, sizeof(int), 1, outfile);
        fwrite(&move->y, sizeof(int), 1, outfile);
        move = move->prev;
    }
  
    // Close file 
    fclose (outfile);

    free(file);

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
    int* h = (int*)malloc(sizeof(int));
    int* win = (int*)malloc(sizeof(int));

    fread(w, sizeof(int), 1, infile);
    fread(h, sizeof(int), 1, infile);
    fread(win, sizeof(int), 1, infile);

    board = board_new(*w, *h, *win);

    free(w);
    free(h);
    free(win);
    
    // Reading players info
    char* name = (char*)malloc(sizeof(char) * 21);
    char piece;

    fread(name, sizeof(char) * 21, 1, infile);
    fread(&piece, sizeof(char), 1, infile);

    player_1 = player_new(name, piece, false);

    fread(name, sizeof(char) * 21, 1, infile);
    fread(&piece, sizeof(char), 1, infile);

    player_2 = player_new(name, piece, false);

    free(name);

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

// Returns a list with all available replay numbers
list_t* replay_list(int last_number)
{
    list_t* list = list_new();

    int number = -1;
    char* file = (char*)malloc(sizeof(char) * 32);

    // Checking files
    while(number < last_number)
    {
        number++;
        strncpy(file, "", 31);
        sprintf(file, "%d", number);
        strcat(file, ".replay\0");
        if(file_exists(file))
            list_add(list, vector_new(number));
    }

    free(file);
    
    return list;
}

// Returns file extension
const char* get_file_ext(char* file)
{
    const char* dot = strrchr(file, '.');
    if(!dot || dot == file) return "";
    return dot + 1;
}

// Returns true if file exists
bool file_exists(char* file)
{
    FILE *f;
    
    if (f = fopen(file, "r")){
        fclose(f);
        return true;
    }
    return false;
}