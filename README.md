# Mancala-console

The board game of Mancala is implemented in a command line interface. Supports human interface, min-max algorithm, and alpha beta pruning.

## Compiling
The project was developed using C++11 and compiled using Make. Compile using commands below:
```bash
make
make release
```

## Strength
The Alpha Beta engine can outperform most human opponents.
Results for Perft-test of depth 10:

<img width="310" alt="image" src="https://github.com/lunathanael/Mancala-console/assets/68858103/6d0c9bc2-470e-48dd-a7ef-cca4ec5981ac">

## Usage
The syntax for the available functions can be found in the headers. To start a game:
```C++
GAMESTATE gamestate;
start_game(&gamestate);
```
The game loop function requires a reference to the two engines and optional parameters.
``` C++
int game_loop(GAMESTATE* gamestate, int (*player_a)(GAMESTATE* gs, int), int (*player_b)(GAMESTATE* gs, int), bool print_output = false, int opt_A = -1, int opt_B = -1);
```

## Commands 
The commands for the console_loop:

quit - exits the console loop and closes the application
newgame - starts a new game
perft x - runs a perft test with depth x
d - displays the current board state
help - displays a help menu for the hole locations
game - starts a game with engines given and optional parameters
``` bash
game eng1 eng2 print_output opt1 opt2
game 3 3 1 16 16
```
Engine indexes:

<img width="431" alt="image" src="https://github.com/lunathanael/Mancala-console/assets/68858103/824e944f-e9e5-4fa2-98db-604fad90c6ce">

simulate - simulates a number of games with given parameters
``` bash
simulate eng1 eng2 number_of_games opt1 opt2
simulate 3 3 100 16 16
```
board - parses a board string
```bash
board 4 4 4 4 4 4 0 4 4 4 4 4 4 0 0
```
## Options
The rules for the game are defined in the defs.h header file, most settings can be changed without issue.

<img width="491" alt="image" src="https://github.com/lunathanael/Mancala-console/assets/68858103/c96d6d7d-07e5-42b2-8b5f-eab45bebc311">

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
