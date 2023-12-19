#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "read_file.h"
#include "game_parser.h"

#define max(x,y) ( x > y ? x : y);

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 1;
	}

	int power_sum = 0;
	struct game st_game;

	char* game_txt = strtok(filebuff, "\n");

	while (game_txt != NULL) {
		memset(&st_game, 0, sizeof(struct game));

		parse_game(&st_game, game_txt);

		int max_red = 0;
		int max_green = 0;
		int max_blue = 0;

		for (int i = 0; i < st_game.num_draws; i++) {
			struct draw* d = &st_game.draws[i];

			max_red = max(max_red, d->red);
			max_green = max(max_green, d->green);
			max_blue = max(max_blue, d->blue);
		}

		if (max_red == 0) {
			max_red = 1;
		}
		if (max_green == 0) {
			max_green = 1;
		}
		if (max_blue == 0) {
			max_blue = 1;
		}

		power_sum += max_red * max_green * max_blue;

		free(st_game.draws);
		game_txt = strtok(NULL, "\n");
	}

	printf("%d\n", power_sum);

	free(filebuff);
	return 0;
}


