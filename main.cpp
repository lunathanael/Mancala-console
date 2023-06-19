#include <iostream>
#include "defs.h"
#include <algorithm>

using namespace std;


void print_number(int number) {
	cout << number;
	if (number < 10) {
		cout << ' ';
	}
	return;
}


void print_board(GAMESTATE* gamestate) {
	int current_hole_index;


	cout << "+---+--+--+--+--+--+--+---+" << endl;

	// Print player B side
	cout << "|   ";
	current_hole_index = PLAYER_TO_STORE_INDEX[PLAYER_B] - 1;
	for (; current_hole_index > PLAYER_TO_STORE_INDEX[PLAYER_A]; --current_hole_index) {
		cout << "|";
		print_number(gamestate->board[current_hole_index]);
	}
	cout << "|   |" << endl;

	// Print Stores
	cout << "|";
	if (gamestate->board[PLAYER_TO_STORE_INDEX[PLAYER_B]] < 10) {
		cout << ' ';
	}
	cout << gamestate->board[PLAYER_TO_STORE_INDEX[PLAYER_B]];
	cout << " +--+--+--+--+--+--+ ";
	print_number(gamestate->board[PLAYER_TO_STORE_INDEX[PLAYER_A]]);
	cout << "|" << endl;

	// Print player A side
	cout << "|   ";
	current_hole_index = 0;
	for (; current_hole_index < PLAYER_TO_STORE_INDEX[PLAYER_A]; ++current_hole_index) {
		cout << "|";
		print_number(gamestate->board[current_hole_index]);
	}
	cout << "|   |" << endl;

	cout << "+---+--+--+--+--+--+--+---+" << endl;

	return;
}


void start_game(GAMESTATE* gs) {
	int current_hole_index;

	gs->current_player = PLAYER_A;

	// Setup board
	current_hole_index = 0;
	
	fill_n(&gs->board[current_hole_index], NUMBER_OF_HOUSES_PER_SIDE, STARTING_SEEDS_PER_HOUSE);


	current_hole_index = PLAYER_TO_STORE_INDEX[PLAYER_A] + 1;

	fill_n(&gs->board[current_hole_index], NUMBER_OF_HOUSES_PER_SIDE, STARTING_SEEDS_PER_HOUSE);
	
	return;
}


void do_capture(GAMESTATE* gs, int hole_index) {
	int opposite_hole_index = ((2 * PLAYER_TO_STORE_INDEX[PLAYER_A]) - hole_index);
	int seed_count = gs->board[opposite_hole_index];
	gs->board[PLAYER_TO_STORE_INDEX[gs->current_player]] += seed_count;
	gs->board[opposite_hole_index] = 0;
	return;
}


bool sowing(GAMESTATE*gs, int hole_index) {
	int seed_count = gs->board[hole_index];
	gs->board[hole_index] = 0;
	while (seed_count > 0) {
		++hole_index;

		if (hole_index == NUMBER_OF_TOTAL_HOLES) {
			hole_index = 0;
		}
		int opposite_player_index = (gs->current_player ^ 1);
		if (hole_index == PLAYER_TO_STORE_INDEX[opposite_player_index]) {
			continue;
		}

		++gs->board[hole_index];
		--seed_count;
	}


	if (hole_index = PLAYER_TO_STORE_INDEX[gs->current_player]) {
		return true;
	}
	else if (ALLOW_CAPTURES) {
		if (gs->board[hole_index] == 1) {
			do_capture(gs, hole_index);
		}
	}

	return false;
}

int game_cycle(GAMESTATE*gs) {
	return 0;
}


int main() {
	GAMESTATE new_game;
	start_game(&new_game);
	print_board(&new_game);

	return 0;
}