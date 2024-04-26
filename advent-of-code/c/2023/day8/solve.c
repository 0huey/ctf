#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "read_file.h"
#include "parser.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 2;
	}

	map_t map;
	parse_map(&map, filebuff);

	free(filebuff);

	size_t steps = 0;
	char* LR_ptr = map.LR_str;

	node_t* node = (node_t*) bsearch(
		"AAA",
		map.node_array,
		map.num_nodes,
		sizeof(node_t),
		&node_comparator
	);

	while (node != NULL && strcmp(node->name, "ZZZ") != 0) {
		char* next;

		if (*LR_ptr == 'L') {
			next = node->left;
		}
		else if (*LR_ptr == 'R') {
			next = node->right;
		}
		else {
			printf("invalid LR character %c\n", *LR_ptr);
			return -1;
		}

		node = (node_t*) bsearch(
			next,
			map.node_array,
			map.num_nodes,
			sizeof(node_t),
			&node_comparator
		);

		steps++;
		LR_ptr++;

		if (*LR_ptr == '\0') {
			LR_ptr = map.LR_str;
		}
	}

	printf("%lu\n", steps);

	return 0;
}
