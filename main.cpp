#include <iostream>
#include "defs.h"

using namespace std;

void print_number(int number) {
	cout << number;
	if (number < 10) {
		cout << ' ';
	}
	return;
}


void print_board(BOARD* board) {
	int current_hole_index;


	cout << "+---+--+--+--+--+--+--+---+" << endl;

	// Print player B side
	cout << "|   ";
	current_hole_index = PLAYER_TO_STORE_INDEX[PLAYER_B] - 1;
	for (; current_hole_index > PLAYER_TO_STORE_INDEX[PLAYER_A]; --current_hole_index) {
		cout << "|";
		print_number(board->STONES_ON_HOLE[current_hole_index]);
	}
	cout << "|   |" << endl;

	// Print Stores
	cout << "|";
	if (board->STONES_ON_HOLE[PLAYER_TO_STORE_INDEX[PLAYER_B]] < 10) {
		cout << ' ';
	}
	cout << board->STONES_ON_HOLE[PLAYER_TO_STORE_INDEX[PLAYER_B]];
	cout << " +--+--+--+--+--+--+ ";
	print_number(board->STONES_ON_HOLE[PLAYER_TO_STORE_INDEX[PLAYER_A]]);
	cout << "|" << endl;

	// Print player A side
	cout << "|   ";
	current_hole_index = 0;
	for (; current_hole_index < PLAYER_TO_STORE_INDEX[PLAYER_A]; ++current_hole_index) {
		cout << "|";
		print_number(board->STONES_ON_HOLE[current_hole_index]);
	}
	cout << "|   |" << endl;

	cout << "+---+--+--+--+--+--+--+---+" << endl;

	return;
}

int main() {
	BOARD new_board;
	print_board(&new_board);

	return 0;
}