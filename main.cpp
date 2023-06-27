#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <time.h>
#include <io.h>
#include <map>

#include "defs.h"
#include "gamestate.h"
#include "engine.h"
#include "perft.h"

using namespace std;



enum COMMANDS
{
	U_unrecognized, U_help, U_newgame, U_stop,
	U_perft, U_quit, U_simulate,
	U_display, U_board, U_game
};


static map<string, COMMANDS> MAP_COMMANDS =
{
	{"help", U_help}, {"newgame", U_newgame}, {"stop", U_stop}, {"perft", U_perft},
	{"quit", U_quit}, {"simulate", U_simulate},
	{"d", U_display}, {"board", U_board}, {"game", U_game}
};


enum ENGINES
{
	HUMAN_PLAYER, RANDOM_PLAYER, MIN_MAX_PLAYER, ALPHA_BETA_PLAYER
};


static map<int, int (*)(GAMESTATE* gs, int)> MAP_ENGINES = {
	{HUMAN_PLAYER, human_player}, {RANDOM_PLAYER, random_player},
	{MIN_MAX_PLAYER, min_max_player}, {ALPHA_BETA_PLAYER, alpha_beta_player}
};


vector<string> split_command(const string& command)
{

	stringstream stream(command);
	string intermediate;
	vector<string> tokens;

	while (getline(stream, intermediate, ' '))
	{
		tokens.push_back(intermediate);
	}

	return tokens;
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


int parse_board(GAMESTATE* gs, vector<string> tokens) {
	GAMESTATE gamestate;
	static const int expected_board_size = (NUMBER_OF_TOTAL_HOLES);
	if (tokens.size() != (2 + expected_board_size)) {
		return true;
	}
	int current_number;
	for (int current_index = 0; current_index < expected_board_size; ++current_index) {
		gamestate.board[current_index] = stoi(tokens.at(current_index + 1));
	}
	const int current_player_index = stoi(tokens.back());
	if ((current_player_index != PLAYER_A) and (current_player_index != PLAYER_B)) {
		return true;
	}
	gamestate.current_player = current_player_index;
	update_game_over(&gamestate);

	memcpy(gs, &gamestate, sizeof(gamestate));
	// ex: 4 4 4 4 4 4 0 4 4 4 4 4 4 0 0
	return false;
}


int parse_simulation(vector<string> tokens) {
	int (*player_a)(GAMESTATE * gs, int);
	int (*player_b)(GAMESTATE * gs, int);

	if (MAP_ENGINES.find(stoi(tokens.at(1))) == MAP_ENGINES.end()) {
		return 1;
	}
	else {
		player_a = MAP_ENGINES[stoi(tokens.at(1))];
	}
	if (MAP_ENGINES.find(stoi(tokens.at(2))) == MAP_ENGINES.end()) {
		return 1;
	}
	else {
		player_b = MAP_ENGINES[stoi(tokens.at(2))];
	}

	int trials = stoi(tokens.at(3));
	int opt_a = 0;
	int opt_b = 0;
	if (tokens.size() > 4) {
		opt_a = stoi(tokens.at(4));
		if (tokens.size() == 6) {
			opt_b = stoi(tokens.at(5));
		}
	}

	try {
		simulate_games(player_a, player_b, trials, opt_a, opt_b);
	}
	catch (...) {
		cout << "Simulation failed!\n";
		return 1;
	}
	// simulate 3 3 100 16 16
	return false;
}


int parse_game(GAMESTATE* gamestate, vector<string> tokens) {
	int (*player_a)(GAMESTATE * gs, int);
	int (*player_b)(GAMESTATE * gs, int);

	if (MAP_ENGINES.find(stoi(tokens.at(1))) == MAP_ENGINES.end()) {
		return 1;
	}
	else {
		player_a = MAP_ENGINES[stoi(tokens.at(1))];
	}
	if (MAP_ENGINES.find(stoi(tokens.at(2))) == MAP_ENGINES.end()) {
		return 1;
	}
	else {
		player_b = MAP_ENGINES[stoi(tokens.at(2))];
	}

	int print_output = stoi(tokens.at(3));
	int opt_a = 0;
	int opt_b = 0;
	if (tokens.size() > 4) {
		opt_a = stoi(tokens.at(4));
		if (tokens.size() == 6) {
			opt_b = stoi(tokens.at(5));
		}
	}

	try {
		game_loop(gamestate, player_a, player_b, print_output, opt_a, opt_b);
	}
	catch (...) {
		cout << "Game could not start!\n";
		return 1;
	}
	// game 3 3 100 16 16
	return false;
}


void console_loop() {

	GAMESTATE gamestate;
	bool game_started = false;

	while (1) {
		string input;

		// get user / GUI input
		if (!getline(cin, input))
		{
			// continue the loop
			break;
		}

		// make sure input is available
		if (!input.length())
		{
			// continue the loop
			continue;
		}

		vector<string> tokens = split_command(input);

		switch (MAP_COMMANDS[tokens[0]])
		{
		case (U_quit):
		{
			goto exit_loop;
			break;
		}
		case (U_newgame):
		{
			start_game(&gamestate);
			game_started = true;
			break;
		}
		case (U_perft):
		{
			const int p_depth = stoi(tokens[1]);
			if (!game_started) // call parse position function
			{
				start_game(&gamestate);
				game_started = true;
			}
			perft_test(&gamestate, p_depth);
			break;
		}
		case (U_display):
		{
			print_board(&gamestate);
			break;
		}

		case (U_help):
		{
			print_help();
			break;
		}

		case (U_unrecognized):
		{
			printf("Unknown command\n");
			break;
		}
		case (U_simulate):
		{
			try {
				if (parse_simulation(tokens)) {
					cout << "Invalid simulation commands.\n";
				}
			}
			catch (...) {
				cout << "Could not parse input.\n";
			}
			break;
		}
		case (U_game):
		{
			try {
				if (!game_started) {
					start_game(&gamestate);
				}
				if (parse_game(&gamestate, tokens)) {
					cout << "Invalid game commands.\n";
				}
			}
			catch (...) {
				cout << "Could not parse input.\n";
			}
			break;
		}
		case (U_board):
		{
			try {
				if (parse_board(&gamestate, tokens)) {
					cout << "Board could not be parsed!\n";
				}	
			}
			catch (...) {
				cout << "Invalid input!\n";
			}
			break;
		}
		}
	}
exit_loop:
	return;
}


int main() {
	time_seed = time(NULL);
	srand(time_seed);

	console_loop();
	return 0;
}