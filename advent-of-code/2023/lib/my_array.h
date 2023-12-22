#pragma once

#include <stdint.h>

struct int_array {
	int len;
	int max_len;
	int* array;
};

struct ptr_array {
	int len;
	int max_len;
	void** array;
};

struct uint64_array {
	size_t len;
	size_t max_len;
	uint64_t* array;
};

void init_int_array(struct int_array* a);
void append_int_array(struct int_array* a, int value);
int int_comparator(const void* a, const void* b) ;

void init_ptr_array(struct ptr_array* a);
void append_ptr_array(struct ptr_array* a, void* value);

void init_uint64_array(struct uint64_array* a);
void append_uint64_array(struct uint64_array* a, uint64_t value);
