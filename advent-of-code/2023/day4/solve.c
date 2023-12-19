#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "read_file.h"
#include "my_array.h"
#include "parser.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 2;
	}

	char* card_str = strtok(filebuff, "\n");

	int total_value = 0;

	while (card_str != NULL) {
		int card_value = 0;
		struct card st_card;

		memset(&st_card, 0, sizeof(struct card));

		if (!parse_card(card_str, &st_card)) {
			return 3;
		}

		qsort(&st_card.picked, PICK_NUM_LEN, sizeof(int), int_comparator);

		for (int i = 0; i < WIN_NUM_LEN; i++) {
			if (st_card.winning[i] == 0) {
				continue; //needed for the example because its smaller input
			}

			int* win_num = (int*)bsearch(
				&st_card.winning[i],
				&st_card.picked,
				PICK_NUM_LEN,
				sizeof(int),
				int_comparator
			);

			if (win_num != NULL) {
				if (card_value == 0) {
					card_value++;
				}
				else {
					card_value *= 2;
				}
			}
		}
		total_value += card_value;
		card_str = strtok(NULL, "\n");
	}

	printf("%d\n", total_value);

	free(filebuff);
	return 0;
}
