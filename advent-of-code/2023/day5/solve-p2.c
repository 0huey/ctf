#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
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

	struct uint64_array* seed_ranges = &alm->seeds;

	if ((seed_ranges->len & 1) != 0) {
		puts("Odd number of seeds");
		return 4;
	}

	uint64_t best_seed;
	uint64_t best_location = UINT64_MAX;

	clock_t begin = clock();

	for (size_t s = 0; s < seed_ranges->len; s += 2) {
		size_t seed  = seed_ranges->array[s];
		size_t limit = seed + seed_ranges->array[s+1];

		for (; seed < limit; seed++) {
			uint64_t location = get_seed_location(alm, seed);

			if (location < best_location) {
				best_location = location;
				best_seed = seed;
			}
		}
	}

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("run time %f s\n", time_spent);

	printf("best seed %lu at location %lu\n", best_seed, best_location);

	almanac_free(alm);
	free(alm);

	return 0;
}
