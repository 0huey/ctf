#define HAND_SIZE 5

struct hand_t {
	char cards[HAND_SIZE];
	int bid;
	size_t strength;
};

struct hand_t* parse_hands(char* filebuff, size_t* out_num_hands);
