#ifndef DEFS_H
#define DEFS_H

#define NUMBER_OF_HOUSES_PER_SIDE 6
#define NUMBER_OF_PLAYERS 2
#define STARTING_SEEDS_PER_HOUSE 4
static constexpr int ALLOW_CAPTURES = 1;

static constexpr int NUMBER_OF_TOTAL_HOUSES = (2 * NUMBER_OF_HOUSES_PER_SIDE);
static constexpr int NUMBER_OF_TOTAL_HOLES = (NUMBER_OF_TOTAL_HOUSES + NUMBER_OF_PLAYERS);


static constexpr int PLAYER_TO_STORE_INDEX[2] = { 6, 13 };


/*
STORE (B)
|    B6   B5   B4   B3   B2   B1
[]   ()   ()   ()   ()   ()   ()  []
[]								  []
[]   ()   ()   ()   ()   ()   ()  []
	 A1   A2   A3   A4   A5   A6  |
								STORE (A)
*/

enum PLAYERS{ 
	PLAYER_A, PLAYER_B
};


enum HOLE_REFERENCE {
	A1, A2, A3, A4, A5, A6, B1, B2, B3, B4, B5, B6
};


constexpr int get_hole_side(int hole) {
	return (hole / 6);
};


struct GAMESTATE {
	unsigned int board[NUMBER_OF_TOTAL_HOLES] = {};
	int current_player = PLAYER_A;
};





#endif