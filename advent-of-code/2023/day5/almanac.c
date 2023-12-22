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

struct almanac* almanac_parser(char* input) {
	struct almanac* alm = malloc(sizeof(struct almanac));

	init_uint64_array(&alm->seeds);

	struct map* this_map;

	for (size_t i = 0; i < NUM_MAPS; i++) {
		this_map = &alm->maps[i];

		for (size_t x = 0; x < NUM_MAP_VALUES; x++) {
			init_uint64_array(&this_map->values[x]);
		}
	}

	this_map = NULL;

	char* line = strtok(input, "\n");

	while (line != NULL) {
		if (isdigit(*line)) {
			uint64_t nums[NUM_MAP_VALUES];

			if(sscanf(
			line,
			"%lu %lu %lu",
			&nums[DEST],
			&nums[SOURCE],
			&nums[RANGE]) != 3) {
				printf("Could not parse 3 nums here: %s", line);
				goto ERROR_EXIT;
			}

			if (this_map == NULL) {
				goto ERROR_EXIT;
			}

			for (size_t i = 0; i < NUM_MAP_VALUES; i++) {
				append_uint64_array(&this_map->values[i], nums[i]);
			}
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
			this_map = &alm->maps[SEED_TO_SOIL];
		}
		else if (!strncmp(line, STR_SOIL_FERT, sizeof(STR_SOIL_FERT)-1)) {
			this_map = &alm->maps[SOIL_TO_FERTILIZER];
		}
		else if (!strncmp(line, STR_FERT_WATER, sizeof(STR_FERT_WATER)-1)) {
			this_map = &alm->maps[FERTILIZER_TO_WATER];
		}
		else if (!strncmp(line, STR_WATER_LIGHT, sizeof(STR_WATER_LIGHT)-1)) {
			this_map = &alm->maps[WATER_TO_LIGHT];
		}
		else if (!strncmp(line, STR_LIGHT_TEMP, sizeof(STR_LIGHT_TEMP)-1)) {
			this_map = &alm->maps[LIGHT_TO_TEMPERATURE];
		}
		else if (!strncmp(line, STR_TEMP_HUM, sizeof(STR_TEMP_HUM)-1)) {
			this_map = &alm->maps[TEMPERATURE_TO_HUMIDITY];
		}
		else if (!strncmp(line, STR_HUM_LOC, sizeof(STR_HUM_LOC)-1)) {
			this_map = &alm->maps[HUMIDITY_TO_LOCATION];
		}
		else {
			this_map = NULL;
			printf("passed line: %s\n", line);
		}

		line = strtok(NULL, "\n");
	}

	return alm;

ERROR_EXIT:
	almanac_free(alm);
	free(alm);
	return NULL;
}

void almanac_free(struct almanac* alm) {
	struct uint64_array* a = &alm->seeds;
	free(a->array);

	for (size_t i = 0; i < NUM_MAPS; i++) {
		struct map* this_map = &alm->maps[i];

		for (size_t x = 0; x < NUM_MAP_VALUES; x++) {
			a = &this_map->values[x];
			free(a->array);
		}
	}
}

void almanac_print(const struct almanac* alm) {
	const struct uint64_array* seeds = &alm->seeds;

	const struct map* this_map;
	const struct uint64_array* source;
	const struct uint64_array* dest;
	const struct uint64_array* range;

	puts("verifying parser:");
	printf("seeds: ");

	for (size_t s = 0; s < seeds->len; s++) {
		printf("%lu ", seeds->array[s]);
	}

	printf("\n\n");

	for (size_t m = 0; m < NUM_MAPS; m++) {
		this_map = &alm->maps[m];

		source = &this_map->values[SOURCE];
		dest   = &this_map->values[DEST];
		range  = &this_map->values[RANGE];

		printf("map %lu\n", m);

		for (size_t i = 0; i < source->len; i++) {
			printf("%lu %lu %lu\n", source->array[i], dest->array[i], range->array[i]);
		}
		puts("");
	}
}
