#include <iostream>
#include <string>

#include "defs.h"
#include "gamestate.h"
#include "engine.h"

using namespace std;



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
	GAMESTATE gamestate;
	start_game(&gamestate);
	game_loop(&gamestate, random_player, random_player, true);
	return 0;
}