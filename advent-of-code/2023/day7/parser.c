#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

struct hand_list_t {
	struct hand_list_t* next;
	struct hand_t hand;
};

struct hand_t* parse_hands(char* filebuff, size_t* out_num_hands) {
	struct hand_list_t* head = NULL;
	size_t num_hands = 0;
	struct hand_list_t* tail;

	char* line = strtok(filebuff, "\n");

	while (line != NULL) {
		printf("parsing line %s\n", line);

		if (head == NULL) {
			head = malloc(sizeof(struct hand_list_t));
			head->next = NULL;
			tail = head;
		}
		else {
			tail->next = malloc(sizeof(struct hand_list_t));
			tail = tail->next;
			tail->next = NULL;
		}

		struct hand_t* hand = &tail->hand;

		for (int i = 0; i < HAND_SIZE && *line != '\0'; i++) {
			hand->cards[i] = *line++;
		}

		sscanf(line, "%d", &hand->bid);
		hand->strength = 0;

		num_hands++;

		line = strtok(NULL, "\n");
	}

	// collapse LL to array
	struct hand_t* hand_array = calloc(num_hands, sizeof(struct hand_t));

	int i = 0;

	while (head != NULL) {
		struct hand_t* hand = &head->hand;
		memcpy(&hand_array[i++], hand, sizeof(struct hand_t));
		struct hand_list_t* temp = head;
		head = head->next;
		free(temp);
	}

	*out_num_hands = num_hands;
	return hand_array;
}
