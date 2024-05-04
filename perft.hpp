#ifndef PERFT_HPP
#define PERFT_HPP

#include <time.h>
#include <Windows.h>
#include <iostream>

#include "defs.hpp"
#include "gamestate.hpp"
#include "engine.hpp"

	
void simulate_games(int (*player_a)(GAMESTATE* gs, int), int (*player_b)(GAMESTATE* gs, int), int trials, int opt_A, int opt_B) {
	int a_wins = 0, b_wins = 0, ties = 0, total_games = trials;
	float progress = 0.0;

	int barWidth = 70;
	int segments = 125;
	int seg_len = trials / segments;
	if (seg_len <= 0) {
		seg_len = 1;
	}

	int starttime = GetTickCount();;
	for (int i = 0; i < total_games; ++i) {
		if (i % seg_len == 0) {
			progress = (float)i / (float)total_games;
			std::cout << "[";
			int pos = barWidth * progress;
			for (int j = 0; j < barWidth; ++j) {
				if (j < pos) std::cout << "=";
				else if (j == pos) std::cout << ">";
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
	std::cout << std::endl << "A won " << ((float)a_wins / float(total_games) * 100) << "% of the games. B won " << ((float)b_wins / float(total_games) * 100) << "% of the games.\n";
	std::cout << ((float)ties / float(total_games) * 100) << "% of the games were ties.\n";
	std::cout << trials << " trials took a total of " << time << "ms, " << trials_per_second << " K trials per second.\n";
}


void perft_driver(GAMESTATE* gs, int depth, int* p_nodes)
{
	if (depth == 0)
	{
		++(*p_nodes);
		return;
	}

	int range_hole[6];
	if (gs->current_player == PLAYER_A) {

		std::copy(A_HOLE_RANGE, A_HOLE_RANGE + 6, range_hole);
	}
	else {

		std::copy(B_HOLE_RANGE, B_HOLE_RANGE + 6, range_hole);
	}

	for (int index = 0; index < 6; ++index) {
		int hole_index = range_hole[index];
		if (gs->board[hole_index] == 0) {
			continue;
		}
		copy_gamestate(gs);
		sowing(&gamestate, hole_index, false);
		perft_driver(&gamestate, depth - 1, p_nodes);
	}
}


void perft_test(GAMESTATE* gs, int depth) {
	int p_nodes = 0;
	printf("\n     Performance test\n\n");
	printf("\n     Move:    Nodes:\n");
	int range_hole[6];
	if (gs->current_player == PLAYER_A) {

		std::copy(A_HOLE_RANGE, A_HOLE_RANGE + 6, range_hole);
	}
	else {

		std::copy(B_HOLE_RANGE, B_HOLE_RANGE + 6, range_hole);
	}
	long start = GetTickCount();

	for (int index = 0; index < 6; ++index) {
		int hole_index = range_hole[index];
		if (gs->board[hole_index] == 0) {
			continue;
		}
		copy_gamestate(gs);
		long long cummulative_nodes = p_nodes;
		sowing(&gamestate, hole_index, false);
		perft_driver(&gamestate, depth - 1, &p_nodes);

		long long old_nodes = p_nodes - cummulative_nodes;
		std::string move_string = HOLE_INDEX_TO_STRING[hole_index];
		std::cout << "     " << move_string << "       " << old_nodes << "\n";
		//printf("     %s    %lld\n", (move_string), old_nodes);
	}

	long time = GetTickCount() - start;
	// print results
	printf("\n    Depth: %d ply\n", depth);
	printf("    Nodes: %lld\n", p_nodes);
	printf("     Time: %ld ms\n", time);
	if (time <= 0) {
		time = 1;
	}
	printf("      Nps: %dMN/s\n\n", ((p_nodes / time) / 1000));
}

#endif