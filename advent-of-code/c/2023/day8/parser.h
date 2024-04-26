typedef struct NODE {
	char name[4];
	char left[4];
	char right[4];
} node_t;

typedef struct MAP {
	char* LR_str;
	node_t* node_array;
	size_t num_nodes;
} map_t;

void parse_map(map_t* output, char* input);
void destroy_map(map_t* map);
int node_comparator(const void* p1, const void* p2);
