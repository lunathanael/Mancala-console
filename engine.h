#ifndef ENGINE_H
#define ENGINE_H

#include "defs.h"

int human_player( GAMESTATE* gs, int);
int random_player( GAMESTATE* gs, int);
bool validate_move( GAMESTATE* gamestate, int hole_index);
int min_max_player(GAMESTATE* gs, int depth);
int alpha_beta_player(GAMESTATE* given_gs, int depth);



#endif