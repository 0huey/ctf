#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "my_array.h"
#include "almanac.h"

extern bool DEBUG;

uint64_t get_seed_location(const struct almanac* alm, uint64_t seed) {
	uint64_t source_value = seed;

	if (DEBUG) {
		printf("\nseed %lu\n", source_value);
	}

	for (size_t m = 0; m < NUM_MAPS; m++) {
		const struct ptr_array* map_array = &alm->maps[m];

		/* Implement a kind of binary search on the source value.
		 * Once we find an index with a source value greater than the
		 * search value, we still have to check each range sequentially
		 */

		// start at 3/4
		size_t i = (map_array->len >> 1) + (map_array->len >> 2);
		size_t limit = map_array->len;

		while (i != limit) {
			const struct mapping* map = (const struct mapping*)map_array->array[i];

			if (source_value < map->source) {
				if (i == 0) {
					break;
				}
				limit = i;
				i = i >> 1;
				continue;
			}

			// already checked that source_value >= map->source
			if (source_value < map->source + map->range) {

				uint64_t diff = source_value - map->source;
				source_value = map->dest + diff;
				break;
			}

			/* once the source index is greater than the search value,
			 * we have to check each range one by one because the range
			 * is not predictable
			 */
			i++;
		}
		if (DEBUG) {
			printf("map %lu dest %lu\n", m, source_value);
		}
	}
	return source_value;
}
