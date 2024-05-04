#include "gamestate.hpp"
#include <iostream>
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


	cout << "+---";
	for (int space_index = 0; space_index < NUMBER_OF_HOUSES_PER_SIDE; ++space_index) {
		cout << "+--";
	}
	cout << "+---+\n";

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
	cout << gamestate->board[PLAYER_TO_STORE_INDEX[PLAYER_B]] << ' ';
	for (int space_index = 0; space_index < NUMBER_OF_HOUSES_PER_SIDE; ++space_index) {
		cout << "+--";
	}

	cout << "+ ";
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

	cout << "+---";
	for (int space_index = 0; space_index < NUMBER_OF_HOUSES_PER_SIDE; ++space_index) {
		cout << "+--";
	}

	cout << "+---+\n\n";
	cout << PLAYER_INDEX_TO_STRING[gamestate->current_player] << "'s turn.\n" << endl;

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

	gs->board[PLAYER_TO_STORE_INDEX[PLAYER_A]] = gs->board[PLAYER_TO_STORE_INDEX[PLAYER_B]] = 0;
	gs->game_result = -1;

	return;
}


bool is_valid_move(GAMESTATE* gamestate, int hole_index) {
	int starter;
	if (gamestate->current_player == PLAYER_A) {
		starter = 0;
	}
	else {
		starter = PLAYER_TO_STORE_INDEX[PLAYER_A] + 1;
	}
	if (hole_index < starter or hole_index > (starter + 5)) {
		return false;
	}

	if (gamestate->board[hole_index] == 0) {
		return false;
	}
	return true;
}


int do_capture(GAMESTATE* gs, int hole_index) {
	if (is_valid_move(gs, hole_index) == false) {
		gs->current_player ^= 1;
		return 0;
	}
	int opposite_hole_index = ((2 * PLAYER_TO_STORE_INDEX[PLAYER_A]) - hole_index);
	int seed_count = gs->board[opposite_hole_index];
	if (CAPTURE_BOTH and (seed_count != 0) ) {
		++seed_count;
		--gs->board[hole_index];
	}
	gs->board[PLAYER_TO_STORE_INDEX[gs->current_player]] += seed_count;
	gs->board[opposite_hole_index] = 0;

	gs->current_player ^= 1;
	return seed_count;
}


void undo_capture(GAMESTATE* gs, int hole_index, int seed_count) {
	int opposite_hole_index = ((2 * PLAYER_TO_STORE_INDEX[PLAYER_A]) - hole_index);
	gs->board[opposite_hole_index] += seed_count;
	gs->board[PLAYER_TO_STORE_INDEX[gs->current_player]] -= seed_count;
	return;
}


int sowing(GAMESTATE* gs, int hole_index, bool verify) {
	if (verify) {
		if (not is_valid_move(gs, hole_index)) {
			throw(505);
		}
	}

	int seed_count = gs->board[hole_index];
	gs->board[hole_index] = 0;

	bool first_cycle = true;
	int opposite_player_index = (gs->current_player ^ 1);
	while (seed_count > 0) {
		++hole_index;

		if (hole_index == NUMBER_OF_TOTAL_HOLES) {
			hole_index = 0;
		}
		if (hole_index == PLAYER_TO_STORE_INDEX[gs->current_player]) {
			first_cycle = false;
		}
		if (hole_index == PLAYER_TO_STORE_INDEX[opposite_player_index]) {
			continue;
		}


		++gs->board[hole_index];
		--seed_count;
	}
	if (hole_index == PLAYER_TO_STORE_INDEX[gs->current_player]) {
		if (ALLOW_MULTIPLE_LAPS) {
			return -1;
		}
	}
	else if (gs->board[hole_index] == 1) {
		if (ALLOW_CAPTURES) {
			if (CAPTURE_ON_ONE_CYCLE) {
				if (first_cycle) {
					return do_capture(gs, hole_index);
				}
			}
			else {
				return do_capture(gs, hole_index);
			}
		}
	}
	else {
		if (ALLOW_RELAY_SOWING) {
			sowing(gs, hole_index, false);
			return -2;
		}
	}

	// Next player
	gs->current_player ^= 1;

	return 0;
}


void unsow(GAMESTATE* gs, int hole_index, int seed_count, int move_type) {
	// Last player
	gs->current_player ^= 1;

	gs->board[hole_index] += seed_count;
	while (seed_count > 0) {
		++hole_index;

		if (hole_index == NUMBER_OF_TOTAL_HOLES) {
			hole_index = 0;
		}
		int opposite_player_index = (gs->current_player ^ 1);
		if (hole_index == PLAYER_TO_STORE_INDEX[opposite_player_index]) {
			continue;
		}

		--gs->board[hole_index];
		--seed_count;
	}
	if (move_type > 0) {
		undo_capture(gs, hole_index, move_type);
	}
	return;
}


void clear_side(GAMESTATE* gamestate, int side_to_clear) {
	int starter;
	int cumm_seeds = 0;
	if (side_to_clear == PLAYER_A) {
		starter = 0;
	}
	else {
		starter = PLAYER_TO_STORE_INDEX[PLAYER_A] + 1;
	}
	for (int hole_index = starter; hole_index < starter + 6; ++hole_index) {
		cumm_seeds += gamestate->board[hole_index];
		gamestate->board[hole_index] = 0;
	}
	gamestate->board[PLAYER_TO_STORE_INDEX[side_to_clear]] += cumm_seeds;
	return;
}


void update_game_result(GAMESTATE* gamestate) {
	int player_a_score = gamestate->board[PLAYER_TO_STORE_INDEX[PLAYER_A]];
	int player_b_score = gamestate->board[PLAYER_TO_STORE_INDEX[PLAYER_B]];
	if (player_a_score > player_b_score) {
		gamestate->game_result = PLAYER_A;
	}
	else if (player_b_score > player_a_score) {
		gamestate->game_result = PLAYER_B;
	}
	else {
		gamestate->game_result = NO_PLAYER;
	}
	return;
}


bool update_game_over(GAMESTATE* gamestate) {

	int added_scores[2] = { 0, 0 };
	for (int hole_index = 0; hole_index < 6; ++hole_index) {
		added_scores[PLAYER_A] += gamestate->board[A_HOLE_RANGE[hole_index]];
		added_scores[PLAYER_B] += gamestate->board[B_HOLE_RANGE[hole_index]];
	}
	if (added_scores[PLAYER_A] != added_scores[PLAYER_B]) {
		if (added_scores[PLAYER_A] == 0) {
			clear_side(gamestate, PLAYER_B);
			update_game_result(gamestate);
			return true;
		}
		if (added_scores[PLAYER_B] == 0) {
			clear_side(gamestate, PLAYER_A);
			update_game_result(gamestate);
			return true;
		}
		return false;
	}
	else if (added_scores[PLAYER_A] == 0) {
		update_game_result(gamestate);
		return true;
	}
	return false;
}


int game_loop(GAMESTATE* gamestate, int (*player_a)( GAMESTATE* gs, int), int (*player_b)( GAMESTATE* gs, int), bool print_output, int opt_A, int opt_B)
 {
	while (gamestate->game_result == -1) {
		if (print_output) {
			print_board(gamestate);
		}
		if (update_game_over(gamestate)) {
			break;
		}
		int hole_selected = -1;
		switch (gamestate->current_player) {
		case PLAYER_A:
			hole_selected = player_a(gamestate, opt_A);
			break;
		case PLAYER_B:
			hole_selected = player_b(gamestate, opt_B);
			break;
		}

		
		try {
			sowing(gamestate, hole_selected);
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

		continue;
	}

	if (print_output) {
		print_board(gamestate);
		cout << "Game over!\n";
		switch (gamestate->game_result) {
		case PLAYER_A:
		case PLAYER_B:
			cout << "Game won by: " << PLAYER_INDEX_TO_STRING[gamestate->game_result] << '\n';
			break;
		case NO_PLAYER:
			cout << "Game was a tie!\n";
			break;
		}
	}

	return true;
}

