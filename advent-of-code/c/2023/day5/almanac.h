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

struct mapping {
	uint64_t dest;
	uint64_t source;
	uint64_t range;
};

struct almanac {
	struct uint64_array seeds;
	struct ptr_array maps[NUM_MAPS];
};

struct almanac* almanac_parser(char* input);
void almanac_free(struct almanac* a);
void almanac_print(const struct almanac* alm);
