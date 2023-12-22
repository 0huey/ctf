#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "my_array.h"
#include "almanac.h"

extern bool DEBUG;

uint64_t get_seed_location(const struct almanac* alm, uint64_t seed) {
	uint64_t source_value = seed;

	const struct map* this_map;
	const struct uint64_array* source;
	const struct uint64_array* dest;
	const struct uint64_array* range;

	if (DEBUG) {
		printf("\nseed %lu\n", source_value);
	}

	for (size_t m = 0; m < NUM_MAPS; m++) {
		this_map = &alm->maps[m];

		source = &this_map->values[SOURCE];
		dest   = &this_map->values[DEST];
		range  = &this_map->values[RANGE];

		if (source->len != dest->len || source->len != range->len) {
			/* the length of these arrays should already be the same
			 * because the parser checks and would have failed otherwise
			 * but just in case
			 */
			puts("array length error");
			exit(-1);
		}

		for (size_t i = 0; i < source->len; i++) {

			if (source_value >= source->array[i]
			&& source_value < source->array[i] + range->array[i]) {

				uint64_t diff = source_value - source->array[i];

				source_value = dest->array[i] + diff;

				break;
			}
		}
		if (DEBUG) {
			printf("map %lu dest %lu\n", m, source_value);
		}
	}

	return source_value;
}
