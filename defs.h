#ifndef DEFS_H
#define DEFS_H

#include <map>
#include <string>

#define NUMBER_OF_HOUSES_PER_SIDE 6
#define NUMBER_OF_PLAYERS 2
#define STARTING_SEEDS_PER_HOUSE 4
static constexpr int ALLOW_CAPTURES = 1;
static constexpr int ALLOW_RELAY_SOWING = 0;
static constexpr int ALLOW_MULTIPLE_LAPS = 1;

static constexpr int NUMBER_OF_TOTAL_HOUSES = (2 * NUMBER_OF_HOUSES_PER_SIDE);
static constexpr int NUMBER_OF_TOTAL_HOLES = (NUMBER_OF_TOTAL_HOUSES + NUMBER_OF_PLAYERS);

static constexpr int PLAYER_TO_STORE_INDEX[2] = { 6, 13 };
static std::map<int, std::string> HOLE_INDEX_TO_STRING = {
	{0, "A1"}, {1, "A2"}, {2, "A3"}, {3, "A4"}, {4, "A5"}, {5, "A6"}, {6, "SA"}, {7, "B1"}, {8, "B2"}, {9, "B3"}, {10, "B4"}, {11, "B5"}, {12, "B6"}, {13, "SB"}
};

static std::map<std::string, int> STRING_TO_HOLE_INDEX = {
	{"A1", 0}, {"A2", 1}, {"A3", 2}, {"A4", 3}, {"A5", 4}, {"A6", 5}, {"SA", 6}, {"B1", 7}, {"B2", 8}, {"B3", 9}, {"B4", 10}, {"B5", 11}, {"B6", 12}, {"SB", 13}
};

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


std::string PLAYER_INDEX_TO_STRING[2] = { "Player A", "Player B" };


enum HOLE_REFERENCE {
	A1, A2, A3, A4, A5, A6, B1, B2, B3, B4, B5, B6
};


constexpr int get_hole_side(int hole) {
	return (hole / 6);
};


struct GAMESTATE {
	unsigned int board[NUMBER_OF_TOTAL_HOLES] = {};
	int current_player = PLAYER_A;
	int game_result = 0;
};





#endif