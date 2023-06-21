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
	U_display, U_settings
};


static map<string, COMMANDS> MAP_COMMANDS =
{
	{"help", U_help}, {"newgame", U_newgame}, {"stop", U_stop}, {"perft", U_perft},
	{"quit", U_quit}, {"simulate", U_simulate},
	{"d", U_display}, {"settings", U_settings}
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
		case (U_settings):
		{
			//parse_settings(input);
			break;
		}
		case (U_simulate):
		{
			//parse_simulate(input);
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

	//simulate_games(alpha_beta_player, random_player, 1'000, 14, 1);

	//int input;
	//cin >> input;
	//if (input == 1) {
	//	game_loop(&gamestate, alpha_beta_player, human_player, true, 16, 1);
	//}
	//else {
	//	game_loop(&gamestate, human_player, alpha_beta_player, true, 16, 16);
	//}


	return 0;
}