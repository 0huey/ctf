#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "read_file.h"
#include "my_array.h"
#include "almanac.h"
#include "seed_eval.h"

bool DEBUG = false;

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 2;
	}

	if (argc >= 3 && strcmp(argv[2], "--debug") == 0) {
		DEBUG = true;
	}

	struct almanac* alm = almanac_parser(filebuff);

	free(filebuff);

	if (alm == NULL) {
		return 3;
	}

	if (DEBUG) {
		almanac_print(alm);
	}

	struct uint64_array* seeds = &alm->seeds;

	uint64_t best_seed;
	uint64_t best_location = UINT64_MAX;

	for (size_t s = 0; s < seeds->len; s++) {
		uint64_t location = get_seed_location(alm, seeds->array[s]);

		if (location < best_location) {
			best_location = location;
			best_seed = seeds->array[s];
		}
	}

	printf("best seed %lu at location %lu\n", best_seed, best_location);

	almanac_free(alm);
	free(alm);

	return 0;
}
