#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <Windows.h>

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


void benchmark(int (*player_a)(GAMESTATE* gs, int), int (*player_b)(GAMESTATE* gs, int), int trials, int opt_A = -1, int opt_B = -1) {
	int a_wins = 0, b_wins = 0, ties = 0, total_games = trials;
	float progress = 0.0;

	int barWidth = 70;
	int segments = 125;
	int seg_len = trials / segments;

	int starttime = GetTickCount();;
	for (int i = 0; i < total_games; ++i) {
		if (i % seg_len == 0) {
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
		game_loop(&new_game, player_a, player_b, false, opt_A, opt_B);
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
	int time = GetTickCount() - starttime;
	double trials_per_second = (((double)trials / (double)time));
	cout << endl << "A won " << ((float)a_wins / float(total_games) * 100) << "% of the games. B won " << ((float)b_wins / float(total_games) * 100) << "% of the games.\n";
	cout << ((float)ties / float(total_games) * 100) << "% of the games were ties.\n";
	cout << trials << " trials took a total of " << time << "ms, " << trials_per_second << " K trials per second.\n";
}


int main() {
	time_seed = time(NULL);
	srand(time_seed);

	//benchmark(alpha_beta_player, random_player, 1'000, 10, 1);
	GAMESTATE gamestate;
	start_game(&gamestate);

	game_loop(&gamestate, alpha_beta_player, alpha_beta_player, true, 10, 1);

	return 0;
	int depth = 1;
	while (1) {
		cout << alpha_beta_player(&gamestate, depth++) << endl;
	}

	game_loop(&gamestate, alpha_beta_player, random_player, true, 6);
	return 0;
}