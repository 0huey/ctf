#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

void parse_map(map_t* output, char* input) {
	char* p = input;
	size_t LR_str_len = 0;

	while (*p != '\n' && *p != '\0') {
		LR_str_len++;
		p++;
	}

	output->LR_str = malloc(LR_str_len + 1);
	memcpy(output->LR_str, input, LR_str_len);
	output->LR_str[LR_str_len] = '\0';

	while (*p == '\n' && *p != '\0') {
		p++;
	}

	input = p;
	p = strchr(p, '\n');
	output->num_nodes = 0;

	while (p != NULL) {
		output->num_nodes++;
		p = strchr(++p, '\n');
	}

	output->node_array = calloc(output->num_nodes, sizeof(node_t));
	node_t* node = output->node_array;

	p = strtok(input, "\n");

	while (p != NULL) {
		int num_written = sscanf(p, "%3s = (%3s, %3s)",
			node->name,
			node->left,
			node->right
		);

		if (num_written != 3) {
			printf("could not parse line %s\n", p);
		}

		p = strtok(NULL, "\n");
		node++;
	}

	qsort(output->node_array, output->num_nodes, sizeof(node_t), &node_comparator);
}

void destroy_map(map_t* map) {
	free(map->LR_str);
	free(map->node_array);
}

int node_comparator(const void* p1, const void* p2) {
	node_t* node1 = (node_t*)p1;
	node_t* node2 = (node_t*)p2;

	return strcmp(node1->name, node2->name);
}
