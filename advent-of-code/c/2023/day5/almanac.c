#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "my_array.h"
#include "almanac.h"

const char STR_SEEDS[] = "seeds:";
const char STR_SEED_SOIL[] = "seed-";
const char STR_SOIL_FERT[] = "soil";
const char STR_FERT_WATER[] = "fert";
const char STR_WATER_LIGHT[] = "water";
const char STR_LIGHT_TEMP[] = "light";
const char STR_TEMP_HUM[] = "temp";
const char STR_HUM_LOC[] = "humid";

int mapping_comparator(const void* p1, const void* p2);

struct almanac* almanac_parser(char* input) {
	struct almanac* alm = malloc(sizeof(struct almanac));

	init_uint64_array(&alm->seeds);

	struct ptr_array* map_array;

	for (size_t i = 0; i < NUM_MAPS; i++) {
		map_array = &alm->maps[i];

		init_ptr_array(map_array);
	}

	map_array = NULL;

	char* line = strtok(input, "\n");

	while (line != NULL) {
		if (isdigit(*line)) {
			struct mapping* values = malloc(sizeof(struct mapping));

			if(sscanf(
			line,
			"%lu %lu %lu",
			&values->dest,
			&values->source,
			&values->range) != 3) {
				printf("Could not parse 3 nums here: %s", line);
				goto ERROR_EXIT;
			}

			if (map_array == NULL) {
				puts("null map ptr");
				goto ERROR_EXIT;
			}

			append_ptr_array(map_array, values);
		}
		else if (!strncmp(line, STR_SEEDS, sizeof(STR_SEEDS)-1)) {

			while (*line != '\0' && !isdigit(*line)) {
				line++;
			}

			uint64_t num;

			while (*line != '\0') {
				if (sscanf(line, "%lu", &num) != 1) {
					break;
				}

				append_uint64_array(&alm->seeds, num);

				//skip to next number
				while (isdigit(*line)) {
					line++;
				}
				while (*line != '\0' && !isdigit(*line)) {
					line++;
				}
			}
		}
		else if (!strncmp(line, STR_SEED_SOIL, sizeof(STR_SEED_SOIL)-1)) {
			map_array = &alm->maps[SEED_TO_SOIL];
		}
		else if (!strncmp(line, STR_SOIL_FERT, sizeof(STR_SOIL_FERT)-1)) {
			map_array = &alm->maps[SOIL_TO_FERTILIZER];
		}
		else if (!strncmp(line, STR_FERT_WATER, sizeof(STR_FERT_WATER)-1)) {
			map_array = &alm->maps[FERTILIZER_TO_WATER];
		}
		else if (!strncmp(line, STR_WATER_LIGHT, sizeof(STR_WATER_LIGHT)-1)) {
			map_array = &alm->maps[WATER_TO_LIGHT];
		}
		else if (!strncmp(line, STR_LIGHT_TEMP, sizeof(STR_LIGHT_TEMP)-1)) {
			map_array = &alm->maps[LIGHT_TO_TEMPERATURE];
		}
		else if (!strncmp(line, STR_TEMP_HUM, sizeof(STR_TEMP_HUM)-1)) {
			map_array = &alm->maps[TEMPERATURE_TO_HUMIDITY];
		}
		else if (!strncmp(line, STR_HUM_LOC, sizeof(STR_HUM_LOC)-1)) {
			map_array = &alm->maps[HUMIDITY_TO_LOCATION];
		}
		else {
			map_array = NULL;
			printf("passed line: %s\n", line);
		}

		line = strtok(NULL, "\n");
	}

	for (size_t i = 0; i < NUM_MAPS; i++) {
		struct ptr_array* map_array = &alm->maps[i];

		qsort(map_array->array, map_array->len, sizeof(void*), &mapping_comparator);
	}

	return alm;

ERROR_EXIT:
	almanac_free(alm);
	free(alm);
	return NULL;
}

void almanac_free(struct almanac* alm) {
	free_uint64_array(&alm->seeds);

	for (size_t i = 0; i < NUM_MAPS; i++) {
		struct ptr_array* map_array = &alm->maps[i];

		for (size_t x = 0; x < map_array->len; x++) {
			struct mapping* map = map_array->array[x];
			free(map);
		}
		free_ptr_array(map_array);
	}
}

void almanac_print(const struct almanac* alm) {
	const struct uint64_array* seeds = &alm->seeds;
	const struct ptr_array* map_array;

	puts("verifying parser:");
	printf("seeds: ");

	for (size_t s = 0; s < seeds->len; s++) {
		printf("%lu ", seeds->array[s]);
	}

	printf("\n\n");

	for (size_t m = 0; m < NUM_MAPS; m++) {
		map_array = &alm->maps[m];

		printf("map %lu\n", m);

		for (size_t i = 0; i < map_array->len; i++) {
			const struct mapping* map = (const struct mapping*)map_array->array[i];
			printf("%lu %lu %lu\n", map->dest, map->source, map->range);
		}
		puts("");
	}
}

int mapping_comparator(const void* p1, const void* p2) {
	const struct mapping* map1 = *(const struct mapping**)p1;
	const struct mapping* map2 = *(const struct mapping**)p2;

	if (map1->source < map2->source) {
		return -1;
	}
	else if (map1->source > map2->source) {
		return 1;
	}
	return 0;
}
