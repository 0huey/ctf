#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "read_file.h"
#include "parser.h"
#include "card_const_p2.h"

int hand_comparator(const void* p1, const void* p2);

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

	char card_counts[NUM_CARDS];

	for (size_t i = 0; i < num_hands; i++) {
		struct hand_t* this_hand = &hands[i];
		char* cards = this_hand->cards; // NOT A STRING

		memset(&card_counts, 0, sizeof(card_counts));

		for (size_t c = 0; c < HAND_SIZE; c++) {
			if (cards[c] >= sizeof(CARD_INDEX_MAP) / sizeof(char)) {
				printf("illegal card in hand %c%c%c%c%c\n",
					cards[0],cards[1],cards[2],cards[3],cards[4]
				);
				exit(-1);
			}

			char card_index = CARD_INDEX_MAP[ cards[c] ];

			if (card_index == -1) {
				printf("illegal card in hand %c%c%c%c%c\n",
					cards[0],cards[1],cards[2],cards[3],cards[4]
				);
				exit(-1);
			}

			card_counts[card_index]++;
		}

		int pairing1 = 0;
		int pairing2 = 0;

		//Jokers are now index 0, skip counting thes as pairs
		for (size_t i = 1; i < NUM_CARDS; i++) {
			if (card_counts[i] <= 1) {
				continue;
			}

			if (pairing1 == 0) {
				pairing1 = card_counts[i];
			}
			else if (pairing1 != 0) {
				pairing2 = card_counts[i];
			}
			else {
				break;
			}
		}

		if (pairing1 == 5) {
			this_hand->strength = FIVE_OF_A_KIND;
		}
		else if (pairing1 == 4) {
			this_hand->strength = FOUR_OF_A_KIND;
		}
		//3 and 2 pairing could be in either order
		else if (pairing1 + pairing2 == 5) {
			this_hand->strength = FULL_HOUSE;
		}
		else if (pairing1 == 3) {
			this_hand->strength = THREE_OF_A_KIND;
		}
		else if (pairing1 == 2 && pairing2 == 2) {
			this_hand->strength = TWO_PAIR;
		}
		else if (pairing1 == 2) {
			this_hand->strength = ONE_PAIR;
		}
		else {
			this_hand->strength = HIGH_CARD;
		}

		char num_jokers = card_counts[ CARD_INDEX_MAP['J'] ];

		if (num_jokers == 5) {
			this_hand->strength = FIVE_OF_A_KIND;
		}
		else {
			while (num_jokers-- > 0) {
				if (this_hand->strength == ONE_PAIR) {
					this_hand->strength = THREE_OF_A_KIND;
				}
				else if (this_hand->strength == TWO_PAIR) {
					this_hand->strength = FULL_HOUSE;
				}
				else if (this_hand->strength == THREE_OF_A_KIND) {
					this_hand->strength = FOUR_OF_A_KIND;
				}
				else {
					this_hand->strength++;
				}
			}
		}

		if (this_hand->strength > FIVE_OF_A_KIND) {
			printf("hand greater than five %c%c%c%c%c\n",
				cards[0],cards[1],cards[2],cards[3],cards[4]
			);
		}

		this_hand++;
	}

	qsort(hands, num_hands, sizeof(struct hand_t), &hand_comparator);

	size_t winnings = 0;

	for (size_t i = 0; i < num_hands; i++) {
		struct hand_t* this_hand = &hands[i];

		size_t rank = i + 1;

		winnings += this_hand->bid * rank;
	}

	printf("%lu\n", winnings);

	free(hands);
	return 0;
}

int hand_comparator(const void* p1, const void* p2) {
	struct hand_t* hand1 = (struct hand_t*)p1;
	struct hand_t* hand2 = (struct hand_t*)p2;

	if (hand1->strength != hand2->strength) {
		return hand1->strength - hand2->strength;
	}

	char* cards1 = hand1->cards;
	char* cards2 = hand2->cards;

	for (int i = 0; i < HAND_SIZE; i++) {
		if (cards1[i] != cards2[i]) {
			return CARD_INDEX_MAP[ cards1[i] ] - CARD_INDEX_MAP[ cards2[i] ];
		}
	}

	return 0;
}
