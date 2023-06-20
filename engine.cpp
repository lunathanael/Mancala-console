#include "engine.h"
#include <random>
#include <iostream>
#include <algorithm>

#include "gamestate.h"

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


// min max stuff

int add_side(GAMESTATE* gamestate) {
	int starter;
	int cumm_seeds = 0;
	int side_to_clear = (gamestate->current_player ^ 1);
	if (side_to_clear == PLAYER_A) {
		starter = 0;
	}
	else {
		starter = PLAYER_TO_STORE_INDEX[PLAYER_A] + 1;
	}
	for (int hole_index = starter; hole_index < starter + 6; ++hole_index) {
		cumm_seeds += gamestate->board[hole_index];
	}
	gamestate->board[PLAYER_TO_STORE_INDEX[side_to_clear ^ 1]] += cumm_seeds;
	return cumm_seeds;
}


int evaluate(GAMESTATE* gs, int added_score=0) {

	int friendly = gs->current_player;
	int opponent = friendly ^ 1;

	int friendly_score = gs->board[PLAYER_TO_STORE_INDEX[friendly]];
	int opponent_score = gs->board[PLAYER_TO_STORE_INDEX[opponent]];


	opponent_score += added_score;

	if (friendly_score > (TOTAL_SEEDS_COUNT / NUMBER_OF_PLAYERS)) {
		return VICTORY_SCORE;
	}
	else if (opponent_score > (TOTAL_SEEDS_COUNT / NUMBER_OF_PLAYERS)) {
		return -VICTORY_SCORE;
	}
	else {
		return (friendly_score - opponent_score);
	}
}



int min_max(GAMESTATE* gs, int depth) {
	if (depth == 0) {
		return evaluate(gs);
	}
	int best_eval = -INFINITY_SCORE;

	int starter = (gs->current_player == PLAYER_A) ? 0 : (PLAYER_TO_STORE_INDEX[PLAYER_A] + 1);
	bool move_found = false;
	for (int hole_index = starter; hole_index < starter + 6; ++hole_index) {
		if (gs->board[hole_index] == 0) {
			continue;
		}
		move_found = true;
		int seed_count = gs->board[hole_index];
		int move_type = sowing(gs, hole_index);
		best_eval = max(best_eval, -min_max(gs, depth - 1));
		unsow(gs, hole_index, seed_count, move_type);
	}

	if (move_found == false) {
		int added_seeds = add_side(gs);
		return evaluate(gs, added_seeds);
	}

	return best_eval;
}


int min_max_player(GAMESTATE* gs, int depth) {
	int best_hole = -1;

	GAMESTATE search_gs;
	memcpy(&search_gs, gs, sizeof(*gs));

	int best_eval = (-INFINITY_SCORE);
	int starter = (search_gs.current_player == PLAYER_A) ? 0 : (PLAYER_TO_STORE_INDEX[PLAYER_A] + 1);
	for (int hole_index = starter; hole_index < starter + 6; ++hole_index) {
		if (search_gs.board[hole_index] == 0) {
			continue;
		}
		int seed_count = search_gs.board[hole_index];
		int move_type = sowing(&search_gs, hole_index);
		int eval = -min_max(&search_gs, depth - 1);
		if (eval > best_eval) {
			best_eval = eval;
			best_hole = hole_index;
		}
		unsow(&search_gs, hole_index, seed_count, move_type);
	}

	return best_hole;
}