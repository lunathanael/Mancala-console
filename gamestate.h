#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <iostream>
#include "defs.h"

using namespace std;

void start_game(GAMESTATE* gs);
int game_loop(GAMESTATE* gamestate, int (*player_a)(GAMESTATE* gs, int), int (*player_b)(GAMESTATE* gs, int), bool print_output = false, int opt_A = -1, int opt_B = -1);


#endif