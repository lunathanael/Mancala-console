#include <iostream>
#include "defs.h"
#include <algorithm>
#include <string>

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

	cout << "+---+--+--+--+--+--+--+---+\n" << endl;

	return;
}

void print_help() {
	int current_hole_index;


	cout << "+---+--+--+--+--+--+--+---+" << endl;

	// Print player B side
	cout << "|   ";
	current_hole_index = PLAYER_TO_STORE_INDEX[PLAYER_B] - 1;
	for (; current_hole_index > PLAYER_TO_STORE_INDEX[PLAYER_A]; --current_hole_index) {
		cout << "|";
		cout << HOLE_INDEX_TO_STRING[current_hole_index];
	}
	cout << "|   |" << endl;

	// Print Stores
	cout << "|";
	cout << HOLE_INDEX_TO_STRING[PLAYER_TO_STORE_INDEX[PLAYER_B]];
	cout << " +--+--+--+--+--+--+ ";
	cout << HOLE_INDEX_TO_STRING[PLAYER_TO_STORE_INDEX[PLAYER_A]];
	cout << "|" << endl;

	// Print player A side
	cout << "|   ";
	current_hole_index = 0;
	for (; current_hole_index < PLAYER_TO_STORE_INDEX[PLAYER_A]; ++current_hole_index) {
		cout << "|";
		cout << HOLE_INDEX_TO_STRING[current_hole_index];
	}
	cout << "|   |" << endl;

	cout << "+---+--+--+--+--+--+--+---+\n" << endl;

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
	// verify
	int starter;
	if (gs->current_player == PLAYER_A) {
		starter = 0;
	}
	else {
		starter = PLAYER_TO_STORE_INDEX[PLAYER_A] + 1;
	}
	if (hole_index < starter or hole_index > (starter + 5)) {
		throw (505);
	}

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


	if (hole_index == PLAYER_TO_STORE_INDEX[gs->current_player]) {
		return true;
	}
	if (ALLOW_CAPTURES) {
		if (gs->board[hole_index] == 1) {
			do_capture(gs, hole_index);
		}
	}

	return false;
}

int human_player(GAMESTATE* gs) {
	cout << "Player " << PLAYER_INDEX_TO_STRING[gs->current_player] << "'s turn.\n";
	input_loop:
	cout << "Print the location of the hole to move: ";
	int hole_index = -1;
	string input;
	cin >> input;
	hole_index = STRING_TO_HOLE_INDEX[input];
	cout << endl;

	int starter;
	if (gs->current_player == PLAYER_A) {
		starter = 0;
	}
	else {
		starter = PLAYER_TO_STORE_INDEX[PLAYER_A] + 1;
	}
	if (hole_index < starter or hole_index >(starter + 5)) {
		cout << "Invalid hole!\n";
		goto input_loop;
	}

	return hole_index;
}

int game_loop(GAMESTATE* gamestate, int (*player_a)(GAMESTATE*gs), int (*player_b)(GAMESTATE*gs), bool print_output = false) {
	while (not gamestate->game_result) {
		if (print_output) {
			print_board(gamestate);
		}
		int hole_selected = -1;
		switch (gamestate->current_player) {
		case PLAYER_A:
			hole_selected = player_a(gamestate);
			break;
		case PLAYER_B:
			hole_selected = player_b(gamestate);
			break;
		}

		bool multiple_laps = false;
		try {
			multiple_laps = sowing(gamestate, hole_selected);
		}
		catch (...) {
			cout << "MOVE ERROR!\n";
			cout << "Tried move: " << HOLE_INDEX_TO_STRING[hole_selected] << " in position. \n";
			print_board(gamestate);
			return 2;
		}
		if (print_output) {
			cout << "Hole selected: " << HOLE_INDEX_TO_STRING[hole_selected] << ".\n";
		}

		if (ALLOW_MULTIPLE_LAPS) {
			if (multiple_laps) {
				continue;
			}
		}
		else {
			gamestate->current_player ^= 1;
		}
		continue;
	}
	return true;
}


int main() {
	GAMESTATE new_game;
	start_game(&new_game);
	print_help();
	game_loop(&new_game, human_player, human_player, true);

	return 0;
}