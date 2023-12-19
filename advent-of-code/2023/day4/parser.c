#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

bool parse_card(char* card_str, struct card* st_card) {
	bool got_id = false;
	bool got_winning = false;
	int i = 0;

	if (strstr(card_str, "Card ") != card_str) {
		return false;
	}

	char* c = card_str + 5; // len "Card "

	while (*c != '\0') {
		if (isdigit(*c)) {
			int* addr;

			if (!got_id) {
				addr = &st_card->id;
				got_id = true;
			}
			else if (!got_winning && i >= WIN_NUM_LEN) {
				puts("Winning numbers too long");
				return false;
			}
			else if (!got_winning) {
				addr = &st_card->winning[i++];
			}
			else if (i >= PICK_NUM_LEN) {
				puts("Picked numbers too long");
				return false;
			}
			else {
				addr = &st_card->picked[i++];
			}

			sscanf(c, "%d", addr);

			while (isdigit(*c)) {
				c++;
			}
		}
		else if (*c == '|') {
			got_winning = true;
			i = 0;
			c++;
		}
		else {
			c++;
		}
	}
	return true;
}
