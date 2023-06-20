#include <iostream>
#include "defs.h"
#include <algorithm>
#include <string>
#include <random>
#include <time.h>

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


bool is_valid_move(GAMESTATE* gamestate, int hole_index) {
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


void do_capture(GAMESTATE* gs, int hole_index) {
	int opposite_hole_index = ((2 * PLAYER_TO_STORE_INDEX[PLAYER_A]) - hole_index);
	int seed_count = gs->board[opposite_hole_index];
	gs->board[PLAYER_TO_STORE_INDEX[gs->current_player]] += seed_count;
	gs->board[opposite_hole_index] = 0;
	return;
}


bool sowing(GAMESTATE*gs, int hole_index) {
	if (not is_valid_move(gs, hole_index)) {
		throw(505);
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


int random_player(GAMESTATE* gs) {
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
	if (not is_valid_move(gs, hole_index)) {
		goto find_loop;
	}
	return hole_index;
}


int human_player(GAMESTATE* gs) {
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
	if (not is_valid_move(gs, hole_index)) {
		cout << "Invalid hole!\n";
		goto input_loop;
	}

	return hole_index;
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
	gamestate->board[PLAYER_TO_STORE_INDEX[side_to_clear ^ 1]] += cumm_seeds;
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
	bool game_over[2] = { true, true };
	for (int hole_index = 0; hole_index < 6; ++hole_index) {
		if (gamestate->board[hole_index] != 0) {
			game_over[0] = false;
		}
	}
	for (int hole_index = 7; hole_index < 13; ++hole_index) {
		if (gamestate->board[hole_index] != 0) {
			game_over[1] = false;
		}
	}

	if (game_over[0] or game_over[1]) {
		if (game_over[0] and game_over[1]) {

		}
		else {
			if (game_over[0]) {
				clear_side(gamestate, PLAYER_B);
			}
			else {
				clear_side(gamestate, PLAYER_A);
			}
		}
		update_game_result(gamestate);
		return true;
	}
	else {
		return false;
	}
}


int game_loop(GAMESTATE* gamestate, int (*player_a)(GAMESTATE*gs), int (*player_b)(GAMESTATE*gs), bool print_output = false) {
	while (gamestate->game_result == -1) {
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

		if (update_game_over(gamestate)) {
			break;
		}
		if (ALLOW_MULTIPLE_LAPS) {
			if (multiple_laps) {
				continue;
			}
		}
		// Next player
		gamestate->current_player ^= 1;
		continue;
	}

	if (print_output) {
		print_board(gamestate);
		cout << "Game over!\n";
		switch (gamestate->game_result) {
		case PLAYER_A:
		case PLAYER_B:
			cout << "Game won by: " << PLAYER_INDEX_TO_STRING[gamestate->game_result];
			break;
		case NO_PLAYER:
			cout << "Game was a tie!\n";
			break;
		}
	}

	return true;
}

void benchmark_random() {
	int a_wins = 0, b_wins = 0, ties = 0, total_games = 100'000;
	float progress = 0.0;
	int barWidth = 70;
	for (int i = 0; i < total_games; ++i) {
		if (i % 2047 == 0) {
			progress = (float)i / (float)total_games;
			std::cout << "[";
			int pos = barWidth * progress;
			for (int i = 0; i < barWidth; ++i) {
				if (i < pos) std::cout << "=";
				else if (i == pos) std::cout << ">";
				else std::cout << " ";
			}
			std::cout << "] " << int(progress * 100.0) << " %\r";
			std::cout.flush();
		}


		GAMESTATE new_game;
		start_game(&new_game);
		game_loop(&new_game, random_player, random_player, false);
		if (new_game.game_result == PLAYER_A) {
			++a_wins;
		}
		else if (new_game.game_result == PLAYER_B) {
			++b_wins;
		}
		else {
			++ties;
		}
		continue;
	}
	cout << endl << "A won " << ((float)a_wins / float(total_games) * 100) << "% of the games. B won " << ((float)b_wins / float(total_games) * 100) << "% of the games.\n";
	cout << ((float)ties / float(total_games) * 100) << "% of the games were ties.";
}

int main() {
	srand(time(NULL));
	GAMESTATE gamestate;
	game_loop(&gamestate, random_player, random_player, true);
	return 0;
}