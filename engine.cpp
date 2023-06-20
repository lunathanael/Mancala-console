#include "engine.h"
#include <random>
#include <time.h>
#include <iostream>

using namespace std;

bool validate_move(GAMESTATE* gamestate, int hole_index) {
	int starter;
	if (gamestate->current_player == PLAYER_A) {
		starter = 0;
	}
	else {
		starter = PLAYER_TO_STORE_INDEX[PLAYER_A] + 1;
	}
	if (hole_index < starter or hole_index >(starter + 5)) {
		return false;
	}

	if (gamestate->board[hole_index] == 0) {
		return false;
	}
	return true;
}

int random_player(GAMESTATE* gs, int) {
	srand(time(NULL));
	int starter, hole_index;
	if (gs->current_player == PLAYER_A) {
		starter = 0;
	}
	else {
		starter = PLAYER_TO_STORE_INDEX[PLAYER_A] + 1;
	}
	int lb = starter, ub = starter + 5;

find_loop:
	hole_index = (rand() % (ub - lb + 1)) + lb;
	if (not validate_move(gs, hole_index)) {
		goto find_loop;
	}
	return hole_index;
}


int human_player(GAMESTATE* gs, int) {
	cout << "Player " << PLAYER_INDEX_TO_STRING[gs->current_player] << "'s turn.\n";
input_loop:
	cout << "Print the location of the hole to move: ";
	int hole_index = -1;
	string input;
	cin >> input;
	cout << endl;

	if (STRING_TO_HOLE_INDEX.find(input) == STRING_TO_HOLE_INDEX.end()) {
		cout << "Invalid input!\n";
		goto input_loop;
	}
	else {
		hole_index = STRING_TO_HOLE_INDEX[input];
	}
	if (not validate_move(gs, hole_index)) {
		cout << "Invalid hole!\n";
		goto input_loop;
	}

	return hole_index;
}



int min_max_player(GAMESTATE* gamestate, int depth) {
	return 0;
}