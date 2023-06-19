#ifndef DEFS_H
#define DEFS_H

#define NUMBER_OF_HOUSES_PER_SIDE 6
#define NUMBER_OF_PLAYERS 2

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

enum PLAYERS{ PLAYER_A, PLAYER_B};


typedef struct BOARD { unsigned int STONES_ON_HOLE[NUMBER_OF_TOTAL_HOLES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; } BOARD;






#endif