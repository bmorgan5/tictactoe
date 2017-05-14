/*******************************************************************************
* FILE NAME : tictactoe.c
* DESCRIPTION : It tic-tac-toe's
* AUTHORS : Benjamin Morgan
* DATE : April 20, 2015
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "tictactoe.h"

int main(int argc, char* argv[])
{

	srand(time(NULL));

	struct player p1;
	struct player p2;
	char game_board[GAME_SPACES];

	init_players(&p1, &p2);
	init_game_board(game_board);

	struct player* cur_player = &p2; // Set cur_player to P2 so that P1 goes first

	printf("So you want to play Tic-tac-toe...\n");
	
	bool game_over = false;	
	int move_count = 0;
	while(!game_over) //main game loop
	{
		// Alternate whose turn it is
		if(cur_player == &p1)
		{
			cur_player = &p2;
		}
		else
		{
			cur_player = &p1;
		}

		print_game_board(game_board);

		int space = cur_player->take_turn(cur_player, game_board);

		int win = check_win(game_board, space);

		move_count++;

		if(win == 1)
		{
			game_over = true;
			printf("\nCongrats! %c's won!\n", cur_player->mark);
		}
		else if(move_count == GAME_SPACES)
		{
			game_over = true;
			printf("\nCat's game!\n\n");
		}

	}

	print_game_board(game_board);

	return 0;
}

// Checks if the player has won the game
// Inputs: The current game board, and the space that the cur_player selected
// Outputs: 1 if win, 0 if not; -1 on error
int check_win(const char* board, int space)
{
	if(board == NULL) return -1;

	if(space < 1 || space > 9) return -1;

	int i = space-1;
	int row = i / 3;
	int col = i - 3*row;
	char c = board[i];

	//Check row
	bool row_win = true;
	for(int x=0; x<3; x++)
	{
		if(board[3*row + x] != c) row_win = false;
	}

	//Check col
	bool col_win = true;
	for(int y=0; y<3; y++)
	{
		if(board[col + 3*y] != c) col_win = false;
	}

	//Check diag
	bool diag_win = true;
	if(i%2 == 1) //Not on the diag
	{
		diag_win = false;
	}
	else
	{
		bool diag1_win = true;
		bool diag2_win = true;
		for(int z=0; z<3; z++)
		{
			if(board[4*z] != c) diag1_win = false;
			if(board[2*z + 2] != c) diag2_win = false;
		}

		if(i == 0 || i == 8)
		{
			diag_win = diag1_win;
		}
		else if(i == 2 || i == 6)
		{
			diag_win = diag2_win;
		}
		else if(i == 4)
		{
			diag_win = diag1_win || diag2_win;
		}

	}
	
	if(row_win || col_win || diag_win) return 1;

	return 0;

}


// Reads input from console, checks that the move is valid, and
// modifies the game board accordingly
// Inputs: The current player & the current game board
// Returns: The space that the user selected (1-9) on success;
//         -1 on error
int take_turn_human(const struct player* player, char* board)
{
	if(player == NULL || board == NULL) return -1;

	int space;
	do
	{
		printf("It is %c's turn.\n", player->mark);
		printf("Please enter the number of the space where you would like to place your mark: ");
		space = get_int_from_user();
	}
	while(!validate_play(board, space));

	board[space-1] = player->mark;

	return space;

}

// Makes a random move & updates the game board accordingly
// Delays 1 second so 
// Inputs: The current player & the current game board
// Returns: The space that the computer selected (1-9) on success;
//          -1 on error
int take_turn_ai(const struct player* player, char* board)
{
	if(player == NULL || board == NULL) return -1;

	printf("It is %c's turn.\n", player->mark);

	// Select random space
	int space = (rand() % 9) + 1;
	while(board[space-1] == 'X' || board[space-1] == 'O')
	{
		space = (space % 9) + 1; // Increment space until next free space is found if necessary
	}

	board[space-1] = player->mark;

	sleep(1);

	printf("The computer selected space %d\n", space);

	return space;

}

// Validates that the user's selection is a legal move
// Inputs: The game board, and the space that the user selected
// Returns: 1 if valid, 0 if not; -1 on error
int validate_play(const char* board, int space)
{

	if(board == NULL)
	{
		return -1;
	}
	else if(space < 1 || space > 9)
	{
		printf("Sorry, that is not a valid space.\n");
		return 0;	
	}
	else if(board[space-1] == 'X' || board[space-1] == 'O')
	{
		printf("Sorry, %d is already taken.\n", space);
		return 0;
	}
	else
	{
		return 1;
	}
	
	
}

// Initializes the players to either human or AI
// Inputs: Two player structs for players 1 & 2
//         The number of players
// Returns: 0 on success; -1 on error
int init_players(struct player* p1, struct player* p2)
{

	if(p1 == NULL || p2 == NULL) return -1;

	int num_players = get_num_players();

	p1->mark = 'X';
	p2->mark = 'O';

	int err = 0;
	switch(num_players)
	{
		case 0:
			p1->take_turn = &take_turn_ai;
			p2->take_turn = &take_turn_ai;
			break;
		case 1:
			p1->take_turn = &take_turn_human;
			p2->take_turn = &take_turn_ai;
			break;
		case 2:
			p1->take_turn = &take_turn_human;
			p2->take_turn = &take_turn_human;
			break;
		default:
			err = -1;
			break;
	}

	return err;	
}


// Initializes the game board to have the the numbers 1-9
// Inputs: The game board
// Returns: 0 on success; -1 on error
int init_game_board(char* board)
{
	if(board == NULL) return -1;
	
	for(int i = 0; i < GAME_SPACES; i++)
	{
		board[i] = (char)('1' + i);
	}

	return 0;
}

// Prints the current game board to the console
// Assumes board is a char array of len GAME_SPACES (i.e. len 9)
// Inputs: The current game board
// Returns 0 on success; -1 on error
int print_game_board(const char* board)
{
	if(board == NULL) return -1;

	printf("\n");
	printf(" %c | %c | %c \n", board[0], board[1], board[2]);
	printf("-----------\n");
	printf(" %c | %c | %c \n", board[3], board[4], board[5]);
	printf("-----------\n");
	printf(" %c | %c | %c \n", board[6], board[7], board[8]);
	printf("\n");

	return 0;
}

// Asks for the number of human players:
//   0: Two AI's play against each other
//   1: One human player and one AI player
//   2: Two humans play against each other (or one sad human plays him/her self)
// Returns: The number of human players selected; -1 on error
int get_num_players()
{
	int num_players;

	do
	{		
		printf("Please enter the number of human players (0-2): ");
		num_players = get_int_from_user();
	}
	while (num_players < 0 || num_players > 2);

	printf("You selected %d players\n\n", num_players);

	return num_players;
}

// Attempts to read an integer from stdin
// Returns: The integer read if successful; -1 on error
int get_int_from_user()
{
	char buffer[USR_INPUT_BUF_LEN]; // large enough to determine input error
	int input = -1;

	if(fgets(buffer, USR_INPUT_BUF_LEN, stdin))
	{
		int temp;
		if(sscanf(buffer, "%d", &temp) > 0)
		{
			input = temp;	
		} 
	}

	return input;
}