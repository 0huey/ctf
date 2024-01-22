#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "read_file.h"
#include "parser.h"

char value_map[] = {2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,A,0,0,0,0,0,0,0,0,J,K,0,0,0,0,0,Q,0,0,T};

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 2;
	}

	size_t num_hands;

	struct hand_t* hands = parse_hands(filebuff, &num_hands);

	for (size_t i = 0; i < num_hands; i++) {
		struct hand_t* this_hand = &hands[i];
		char* cards = this_hand->cards; // NOT A STRING


	}

	free(hands);
	return 0;
}
