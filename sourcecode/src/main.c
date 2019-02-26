#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "..\include\game.h"

const char* GAME_VERSION = "0.2";

board_t* game_board;

// Main
int main(int argc, char **argv)
{
	//scanf("%c", testChar);

	player_t* player_1 = player_new("Player 1", 'X', false);
	player_t* player_2 = player_new("Player 2", 'O', false);

	game_board = board_new(4, 3, 3, player_1, player_2);


	int chosen = menu();
	// Switching chosen option
	switch (chosen)
	{
	case 0:
		board_play(game_board);
		printf("# Game ended.");
		break;
	default:
		printf("# See you next time!");
		break;
	}

	return 0;
}
