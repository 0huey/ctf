#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "game_parser.h"

void parse_game(struct game* st_game, char* game_txt) {

	//Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red

	st_game->num_draws = 1;

	char* p = strstr(game_txt, ";");

	while (p != NULL) {
		st_game->num_draws++;
		p = strstr(++p, ";");
	}

	st_game->draws = calloc(st_game->num_draws, sizeof(struct draw));

	memset(st_game->draws, 0, st_game->num_draws * sizeof(struct draw));

	p = game_txt;

	size_t game_strlen = strlen(game_txt);

	while (*p != '\0' && !isdigit(*p)) {
		p++; // skip to game id
	}

	sscanf(p, "%d", &st_game->id);

	while (*p != '\0' && isdigit(*p)) {
		p++; // skip over game id
	}

	char num_str[10] = {0};
	char color[10] = {0};
	size_t draw_i = 0;

	while (p - game_txt < game_strlen) {
		while (*p != '\0' && !isdigit(*p)) {
			p++; // skip to color number
		}

		int num;
		sscanf(p, "%d", &num);

		while (*p != '\0' && !isalpha(*p)) {
			p++; // skip to color
		}

		switch (*p) {
			case 'b':
				st_game->draws[draw_i].blue = num;
				break;
			case 'g':
				st_game->draws[draw_i].green = num;
				break;
			case 'r':
				st_game->draws[draw_i].red = num;
				break;
			default:
		}

		while (*p != '\0' && isalpha(*p)) {
			p++; // skip over color
		}

		if (*p == ';') {
			draw_i++;
		}
	}
}
