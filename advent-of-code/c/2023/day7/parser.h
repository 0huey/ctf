#define HAND_SIZE 5

enum hand_type {
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	FIVE_OF_A_KIND
};

struct hand_t {
	char cards[HAND_SIZE];
	int bid;
	enum hand_type strength;
};

struct hand_t* parse_hands(char* filebuff, size_t* out_num_hands);
