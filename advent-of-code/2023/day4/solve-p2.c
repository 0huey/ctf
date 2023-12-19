#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "read_file.h"
#include "my_array.h"
#include "parser.h"

struct extra_copies {
	struct extra_copies* next;
	int copies;
};

struct extra_copies* ll_add_values(struct extra_copies* s, int count, int copies);
struct extra_copies* ll_drop_head(struct extra_copies* s);
void ll_free(struct extra_copies* s);

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 2;
	}

	char* card_str = strtok(filebuff, "\n");

	struct extra_copies* copies_tracker = NULL;

	int total_cards = 0;

	while (card_str != NULL) {
		struct card st_card;

		memset(&st_card, 0, sizeof(struct card));

		if (!parse_card(card_str, &st_card)) {
			printf("error parsing card:\n%s\n", card_str);
			return 3;
		}

		qsort(&st_card.picked, PICK_NUM_LEN, sizeof(int), int_comparator);

		int card_matches = 0;

		for (int i = 0; i < WIN_NUM_LEN; i++) {
			if (st_card.winning[i] == 0) {
				continue; //needed for the example because its a smaller input
			}

			int* win_num = (int*)bsearch(
				&st_card.winning[i],
				&st_card.picked,
				PICK_NUM_LEN,
				sizeof(int),
				int_comparator
			);

			if (win_num != NULL) {
				card_matches++;
			}
		}

		int num_cards = 1;

		if (copies_tracker != NULL) {
			num_cards += copies_tracker->copies;
		}

		total_cards += num_cards;

		copies_tracker = ll_drop_head(copies_tracker);

		copies_tracker = ll_add_values(copies_tracker, card_matches, num_cards);

		card_str = strtok(NULL, "\n");
	}

	printf("%d\n", total_cards);

	ll_free(copies_tracker);
	copies_tracker = NULL;

	free(filebuff);
	return 0;
}

struct extra_copies* ll_add_values(struct extra_copies* s, int cards, int copies) {
	if (cards == 0 || copies == 0) {
		return s;
	}

	if (s == NULL) {
		s = malloc(sizeof(struct extra_copies));
		s->next = NULL;
		s->copies = 0;
	}

	struct extra_copies* head = s;

	while (cards-- > 0) {
		s->copies += copies;

		if (s->next == NULL) {
			s->next = malloc(sizeof(struct extra_copies));
			s = s->next;
			s->next = NULL;
			s->copies = 0;
		}
		else {
			s = s->next;
		}
	}

	return head;
}

struct extra_copies* ll_drop_head(struct extra_copies* s) {
	if (s == NULL) {
		return NULL;
	}

	struct extra_copies* new_head = s->next;

	free(s);

	return new_head;
}

void ll_free(struct extra_copies* s) {
	if (s != NULL) {
		ll_free(s->next);
		free(s);
	}
}





















