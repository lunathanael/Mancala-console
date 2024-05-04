#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "defs.hpp"

int human_player( GAMESTATE* gs, int);
int random_player( GAMESTATE* gs, int);
bool validate_move( GAMESTATE* gamestate, int hole_index);
int min_max_player(GAMESTATE* gs, int depth);
int alpha_beta_player(GAMESTATE* given_gs, int depth);
int simple_threaded_alpha_beta_player(GAMESTATE* gs, int depth);



#endif