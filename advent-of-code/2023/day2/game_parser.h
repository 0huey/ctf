struct draw {
	int red;
	int green;
	int blue;
};

struct game {
	int id;
	int num_draws;
	struct draw* draws;
};

void parse_game(struct game* parsed_game, char* game_txt);
