#include <my_array.h>

enum map_type {
	SEED_TO_SOIL,
	SOIL_TO_FERTILIZER,
	FERTILIZER_TO_WATER,
	WATER_TO_LIGHT,
	LIGHT_TO_TEMPERATURE,
	TEMPERATURE_TO_HUMIDITY,
	HUMIDITY_TO_LOCATION,
	NUM_MAPS
};

enum map_values {
	DEST,
	SOURCE,
	RANGE,
	NUM_MAP_VALUES
};

struct map {
	struct uint64_array values[NUM_MAP_VALUES];
};

struct almanac {
	struct uint64_array seeds;
	struct map maps[NUM_MAPS];
};

struct almanac* almanac_parser(char* input);
void almanac_free(struct almanac* a);
void almanac_print(const struct almanac* alm);
