#include <iostream>
#include "defs.h"

using namespace std;


void print_board(BOARD* board) {
	int current_hole_index;

	// Print player B side
	cout << "  ";
	current_hole_index = PLAYER_TO_STORE_INDEX[PLAYER_B] - 1;
	for (; current_hole_index > PLAYER_TO_STORE_INDEX[PLAYER_A]; --current_hole_index) {
		cout << board->STONES_ON_HOLE[current_hole_index] << ' ';
	}
	cout << endl;

	// Print Stores
	cout << board->STONES_ON_HOLE[PLAYER_TO_STORE_INDEX[PLAYER_B]] << "                " << board->STONES_ON_HOLE[PLAYER_TO_STORE_INDEX[PLAYER_A]] << endl;
	// Print player A side
}

int main() {
	BOARD new_board;
	print_board(&new_board);

	return 0;
}