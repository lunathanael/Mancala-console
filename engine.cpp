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
	int input = -1;
	cin >> input;
	cout << endl;

	if (gs->current_player == PLAYER_A) {
		--input;
	}
	else {
		input += 6;
	}
	//if (STRING_TO_HOLE_INDEX.find(input) == STRING_TO_HOLE_INDEX.end()) {
	//	cout << "Invalid input!\n";
	//	goto input_loop;
	//}
	//else {
	//	hole_index = STRING_TO_HOLE_INDEX[input];
	//}
	if (validate_move(gs, input) == false) {
		cout << "Invalid input!\n";
		goto input_loop;
	}
	//if (not validate_move(gs, hole_index)) {
	//	cout << "Invalid hole!\n";
	//	goto input_loop;
	//}

	return input;
}


int add_side(GAMESTATE* gamestate) {

	int added_scores[2] = { 0, 0 };
	for (int hole_index = 0; hole_index < 6; ++hole_index) {
		added_scores[PLAYER_A] += gamestate->board[A_HOLE_RANGE[hole_index]];
		added_scores[PLAYER_B] += gamestate->board[B_HOLE_RANGE[hole_index]];
	}
	if (added_scores[PLAYER_A] != added_scores[PLAYER_B]) {
		if (added_scores[PLAYER_A] == 0) {
			return (-added_scores[PLAYER_B]);
		}
		if (added_scores[PLAYER_B] == 0) {
			return added_scores[PLAYER_A];
		}
		return 420;
	}
	else if (added_scores[PLAYER_A] == 0) {
		return 0;
	}
	else { return 420; }
}


int evaluate(GAMESTATE* gs, int added_score=0) {

	int a_score = gs->board[PLAYER_TO_STORE_INDEX[PLAYER_A]];
	int b_score = gs->board[PLAYER_TO_STORE_INDEX[PLAYER_B]];

	if (added_score != 0) {
		if (added_score > 0) {
			a_score += added_score;
		}
		if (added_score < 0) {
			b_score -= added_score;
		}
	}

	if (a_score > WINNING_SCORE) {
		a_score += VICTORY_SCORE;
	}
	if (b_score > WINNING_SCORE) {
		b_score += VICTORY_SCORE;
	}
	return (a_score - b_score);
}


int min_max(GAMESTATE* gs, int depth) {
	int added_seeds = add_side(gs);
	if (added_seeds != 420) {
		return evaluate(gs, added_seeds);
	}
	if (depth == 0) {
		return evaluate(gs);
	}


	int best_eval;
	int range_hole[6];
	if (gs->current_player == PLAYER_A) {
		best_eval = -INFINITY_SCORE;
		copy(A_HOLE_RANGE, A_HOLE_RANGE + 6, range_hole);
		for (int hole_query = 0; hole_query < 6; ++hole_query) {
			int hole_index = range_hole[hole_query];
			if (gs->board[hole_index] == 0) {
				continue;
			}
			copy_gamestate(gs);
			sowing(&gamestate, hole_index, false);
			best_eval = max(min_max(&gamestate, depth - 1), best_eval);
			//unsow(gs, hole_index, seed_count, move_type);

		}
	}
	else {
		best_eval = INFINITY_SCORE;
		copy(B_HOLE_RANGE, B_HOLE_RANGE + 6, range_hole);
		for (int hole_query = 0; hole_query < 6; ++hole_query) {
			int hole_index = range_hole[hole_query];
			if (gs->board[hole_index] == 0) {
				continue;
			}
			copy_gamestate(gs);
			sowing(&gamestate, hole_index, false);
			best_eval = min(min_max(&gamestate, depth - 1), best_eval);
			//unsow(&gs, hole_index, seed_count, move_type);

		}
	}

	return best_eval;
}


int min_max_player(GAMESTATE* gs, int depth) {
	int best_hole = -1;

	int best_eval;
	int range_hole[6];
	if (gs->current_player == PLAYER_A) {
		best_eval = -INFINITY_SCORE;
		copy(A_HOLE_RANGE, A_HOLE_RANGE + 6, range_hole);
		shuffle(range_hole, range_hole + 6, default_random_engine(time_seed));
		for (int hole_query = 0; hole_query < 6; ++hole_query) {
			int hole_index = range_hole[hole_query];
			if (gs->board[hole_index] == 0) {
				continue;
			}

			copy_gamestate(gs);
			sowing(&gamestate, hole_index, false);
			int eval = min_max(&gamestate, depth - 1);
			//unsow(&gs, hole_index, seed_count, move_type);

			if (best_eval < eval) {
				best_eval = eval;
				best_hole = hole_index;
			}
		}
	}
	else {
		best_eval = INFINITY_SCORE;
		copy(B_HOLE_RANGE, B_HOLE_RANGE + 6, range_hole);
		shuffle(range_hole, range_hole + 6, default_random_engine(time_seed));
		for (int hole_query = 0; hole_query < 6; ++hole_query) {
			int hole_index = range_hole[hole_query];
			if (gs->board[hole_index] == 0) {
				continue;
			}

			copy_gamestate(gs);
			sowing(&gamestate, hole_index, false);
			int eval = min_max(&gamestate, depth - 1);
			//unsow(&gs, hole_index, seed_count, move_type);

			if (eval < best_eval) {
				best_eval = eval;
				best_hole = hole_index;
			}
		}
	}
	//cout << "Eval: " << best_eval << ' ' << HOLE_INDEX_TO_STRING[best_hole] << endl;
	return best_hole;
}


int alpha_beta(GAMESTATE* gs, int depth, int alpha, int beta) {
	int added_seeds = add_side(gs);
	if (added_seeds != 420) {
		return evaluate(gs, added_seeds);
	}
	if (depth == 0) {
		return evaluate(gs);
	}


	int best_eval;
	int range_hole[6];
	if (gs->current_player == PLAYER_A) {
		best_eval = -INFINITY_SCORE;
		copy(A_HOLE_RANGE, A_HOLE_RANGE + 6, range_hole);
		for (int hole_query = 0; hole_query < 6; ++hole_query) {
			int hole_index = range_hole[hole_query];
			if (gs->board[hole_index] == 0) {
				continue;
			}
			copy_gamestate(gs);
			sowing(&gamestate, hole_index, false);
			best_eval = max(alpha_beta(&gamestate, depth - 1, alpha, beta), best_eval);
			//unsow(gs, hole_index, seed_count, move_type);

			alpha = max(alpha, best_eval);
			if (best_eval >= beta) {
				break;
			}
		}
	}
	else {
		best_eval = INFINITY_SCORE;
		copy(B_HOLE_RANGE, B_HOLE_RANGE + 6, range_hole);
		for (int hole_query = 0; hole_query < 6; ++hole_query) {
			int hole_index = range_hole[hole_query];
			if (gs->board[hole_index] == 0) {
				continue;
			}
			copy_gamestate(gs);
			sowing(&gamestate, hole_index, false);
			best_eval = min(alpha_beta(&gamestate, depth - 1, alpha, beta), best_eval);
			//unsow(&gs, hole_index, seed_count, move_type);

			beta = min(beta, best_eval);
			if (best_eval <= alpha) {
				break;
			}
		}
	}

	return best_eval;
}


int alpha_beta_player(GAMESTATE* gs, int depth) {
	int best_hole = -1;

	int best_eval;
	int range_hole[6];
	int alpha = -INFINITY_SCORE;
	int beta = INFINITY_SCORE;
	if (gs->current_player == PLAYER_A) {
		best_eval = -INFINITY_SCORE;
		copy(A_HOLE_RANGE, A_HOLE_RANGE + 6, range_hole);
		shuffle(range_hole, range_hole + 6, default_random_engine(time_seed));
		for (int hole_query = 0; hole_query < 6; ++hole_query) {
			int hole_index = range_hole[hole_query];
			if (gs->board[hole_index] == 0) {
				continue;
			}

			copy_gamestate(gs);
			sowing(&gamestate, hole_index, false);
			int eval = alpha_beta(&gamestate, depth - 1, alpha, beta);
			//unsow(&gs, hole_index, seed_count, move_type);

			if (best_eval < eval) {
				best_eval = eval;
				best_hole = hole_index;
			}
			alpha = max(alpha, best_eval);
			if (best_eval >= beta) {
				break;
			}
		}
	}
	else {
		best_eval = INFINITY_SCORE;
		copy(B_HOLE_RANGE, B_HOLE_RANGE + 6, range_hole);
		shuffle(range_hole, range_hole + 6, default_random_engine(time_seed));
		for (int hole_query = 0; hole_query < 6; ++hole_query) {
			int hole_index = range_hole[hole_query];
			if (gs->board[hole_index] == 0) {
				continue;
			}

			copy_gamestate(gs);
			sowing(&gamestate, hole_index, false);
			int eval = alpha_beta(&gamestate, depth - 1, alpha, beta);
			//unsow(&gs, hole_index, seed_count, move_type);

			if (eval < best_eval) {
				best_eval = eval;
				best_hole = hole_index;
			}
			beta = min(beta, best_eval);
			if (best_eval <= alpha) {
				break;
			}
		}
	}
	//cout << "Eval: " << best_eval << ' ' << HOLE_INDEX_TO_STRING[best_hole] << endl;
	return best_hole;
}