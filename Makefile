################################################################################
# FILE NAME : Makefile
# DESCRIPTION : It tic-tac-toe's
# AUTHORS : Benjamin Morgan
# DATE : April 20, 2015
################################################################################

CC=gcc
CFLAGS= -std=c11 -Wall

OBJ = tictactoe.o

tictactoe: $(OBJ)
	$(CC) $(CFLAGS) -o tictactoe $(OBJ)

tictactoe.o: tictactoe.c tictactoe.h
	$(CC) $(CFLAGS) -c tictactoe.c

.PHONY: clean

clean: 
	-rm $(OBJ) tictactoe