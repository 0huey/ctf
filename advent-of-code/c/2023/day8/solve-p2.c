#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "read_file.h"
#include "parser.h"

#define NODE_NAME_LAST_CHAR 2

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

	size_t num_starting_nodes = 0;

	for (size_t i = 0; i < map.num_nodes; i++) {
		if (map.node_array[i].name[NODE_NAME_LAST_CHAR] == 'A') {
			num_starting_nodes++;
		}
	}

	node_t** starting_nodes = calloc(num_starting_nodes, sizeof(node_t*));
	size_t i_starting_nodes = 0;

	for (size_t i_node = 0; i_node < map.num_nodes; i_node++) {
		if (map.node_array[i_node].name[NODE_NAME_LAST_CHAR] == 'A') {
			starting_nodes[i_starting_nodes++] = &map.node_array[i_node];
		}
	}

	size_t steps = 0;
	char* LR_ptr = map.LR_str;

	while (true) {
		steps++;
		bool all_Zs = true;

		for (size_t i = 0; i < num_starting_nodes; i++) {
			const char* next;

			if (*LR_ptr == 'L') {
				next = starting_nodes[i]->left;
			}
			else if (*LR_ptr == 'R') {
				next = starting_nodes[i]->right;
			}
			else {
				printf("invalid LR character %c\n", *LR_ptr);
				return -1;
			}

			starting_nodes[i] = (node_t*) bsearch(
				next,
				map.node_array,
				map.num_nodes,
				sizeof(node_t),
				&node_comparator
			);

			if (starting_nodes[i] == NULL) {
				puts("bsearch returned NULL");
				exit(1);
			}

			if (all_Zs && starting_nodes[i]->name[NODE_NAME_LAST_CHAR] != 'Z') {
				all_Zs = false;
			}
		}

		if (all_Zs) {
			break;
		}

		LR_ptr++;

		if (*LR_ptr == '\0') {
			LR_ptr = map.LR_str;
		}
	}

	printf("%lu\n", steps);

	destroy_map(&map);
	free(starting_nodes);
	return 0;
}
