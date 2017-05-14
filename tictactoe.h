/*******************************************************************************
* FILE NAME : tictactoe.h
* DESCRIPTION : It tic-tac-toe's
* AUTHORS : Benjamin Morgan
* DATE : April 20, 2015
*******************************************************************************/

#ifndef TICTACTOE_H
#define TICTACTOE_H

#define USR_INPUT_BUF_LEN 40
#define GAME_SPACES 9

typedef enum
{
	false,
	true
} bool;

struct player{
	char mark;
	int (*take_turn)(const struct player*, char*);
};

int check_win(const char* board, int space);
int init_game_board(char* board);
int init_players(struct player* p1, struct player* p2);
int get_int_from_user();
int get_num_players();
int take_turn_ai(const struct player* player, char* board);
int take_turn_human(const struct player* player, char* board);
int validate_play(const char* board, int space);
int print_game_board(const char* board);


#endif // TICTACTOE_H