#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "my_array.h"

struct entry {
	struct entry* next;
	int row;
	int col;
	char num[16];
};

static struct entry* ll_append(struct entry* start, struct entry* item);
static void ll_free(struct entry* item);

int* parser(char* schematic) {
	int row = 0;
	int col = 0;
	struct entry temp_entry;
	struct entry* num_list = NULL;
	struct entry* symbol_list = NULL;

	memset(&temp_entry.num, 0, sizeof(temp_entry.num));

	while (*schematic != '\0') {
		if (*schematic == '.') {
			col++;
			schematic++;
		}

		else if (*schematic == '\n') {
			row++;
			col = 0;
			schematic++;
		}

		else if (isdigit(*schematic)) {
			temp_entry.row = row;
			temp_entry.col = col;

			int i = 0;

			while (isdigit(*schematic) && i < sizeof(temp_entry.num)-1) {
				temp_entry.num[i++] = *schematic++;
				col++;
			}

			temp_entry.num[i] = '\0';

			num_list = ll_append(num_list, &temp_entry);
		}

		else if (*schematic == '*') {
			temp_entry.row = row;
			temp_entry.col = col;

			symbol_list = ll_append(symbol_list, &temp_entry);

			col++;
			schematic++;
		}

		else {
			col++;
			schematic++;
		}
	}

	struct int_array num_array;
	init_int_array(&num_array);

	struct entry* s = symbol_list;

	while (s != NULL) {
		struct entry* n = num_list;

		int gear1 = 0;

		while (n != NULL) {
			int n_row_min = n->row - 1;
			int n_row_max = n->row + 1;
			int n_col_min = n->col - 1;
			int n_col_max = n->col + strlen(n->num);

			if (s->row >= n_row_min &&
			s->row <= n_row_max &&
			s->col >= n_col_min &&
			s->col <= n_col_max) {

				int tmp_num;
				sscanf(n->num, "%d", &tmp_num);

				if (gear1 == 0) {
					gear1 = tmp_num;
				}
				else {
					//gear1 is alreay set, so we have valid gear ratio to calc
					append_int_array(&num_array, gear1 * tmp_num);

					break;
				}
			}
			n = n->next;
		}
		s = s->next;
	}

	append_int_array(&num_array, 0); // 0 terminate array;

	ll_free(num_list);
	ll_free(symbol_list);

	return num_array.array;
}

static struct entry* ll_append(struct entry* start, struct entry* new) {
	if (start == NULL) {
		start = malloc(sizeof(struct entry));
		memcpy(start, new, sizeof(struct entry));
		start->next = NULL;
		return start;
	}

	struct entry* end = start;

	while (end->next != NULL) {
		end = end->next;
	}

	end->next = malloc(sizeof(struct entry));

	end = end->next;

	memcpy(end, new, sizeof(struct entry));

	end->next = NULL;

	return start;
}

static void ll_free(struct entry* item) {
	if (item->next != NULL) {
		ll_free(item->next);
	}
	free(item);
}


