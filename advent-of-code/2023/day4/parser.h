#define WIN_NUM_LEN 10
#define PICK_NUM_LEN 25

struct card {
	int id;
	int winning[WIN_NUM_LEN];
	int picked[PICK_NUM_LEN];
};

bool parse_card(char* card_str, struct card* st_card);
