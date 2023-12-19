#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "read_file.h"
#include "game_parser.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 1;
	}

	int possible_games = 0;
	struct game st_game;

	char* game_txt = strtok(filebuff, "\n");

	while (game_txt != NULL) {
		memset(&st_game, 0, sizeof(struct game));

		parse_game(&st_game, game_txt);

		bool possible = true;

		for (int i = 0; i < st_game.num_draws; i++) {
			if (st_game.draws[i].red > 12 ||
			st_game.draws[i].green > 13 ||
			st_game.draws[i].blue > 14) {

				possible = false;
				break;
			}
		}

		if (possible) {
			possible_games += st_game.id;
		}

		free(st_game.draws);
		game_txt = strtok(NULL, "\n");
	}

	printf("%d\n", possible_games);

	free(filebuff);
	return 0;
}


