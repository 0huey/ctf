#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "my_array.h"


#ifndef MY_ARRAY_LEN
#define MY_ARRAY_LEN 20
#endif

void init_int_array(struct int_array* a) {
	a->len = 0;
	a->max_len = MY_ARRAY_LEN;
	a->array = calloc(sizeof(int), MY_ARRAY_LEN);
}

void append_int_array(struct int_array* a, int value) {
	if (a->len == a->max_len) {
		// expand array
		a->max_len += MY_ARRAY_LEN;
		int* tmp = calloc(sizeof(int), a->max_len);
		memcpy(tmp, a->array, sizeof(int) * a->len);
		free(a->array);
		a->array = tmp;
	}

	a->array[a->len++] = value;
}

int int_comparator(const void* a, const void* b) {
	return *(int*)a - *(int*)b;
}

void init_ptr_array(struct ptr_array* a) {
	a->len = 0;
	a->max_len = MY_ARRAY_LEN;
	a->array = calloc(sizeof(void*), MY_ARRAY_LEN);
}

void append_ptr_array(struct ptr_array* a, void* value) {
	if (a->len == a->max_len) {
		a->max_len += MY_ARRAY_LEN;
		void** tmp = calloc(sizeof(void*), a->max_len);
		memcpy(tmp, a->array, sizeof(void*) * a->len);
		free(a->array);
		a->array = tmp;
	}

	a->array[a->len++] = value;
}

void init_uint64_array(struct uint64_array* a) {
	a->len = 0;
	a->max_len = MY_ARRAY_LEN;
	a->array = calloc(sizeof(uint64_t), MY_ARRAY_LEN);
}

void append_uint64_array(struct uint64_array* a, uint64_t value) {
	if (a->len == a->max_len) {
		a->max_len += MY_ARRAY_LEN;
		uint64_t* tmp = calloc(sizeof(uint64_t), a->max_len);
		memcpy(tmp, a->array, sizeof(uint64_t) * a->len);
		free(a->array);
		a->array = tmp;
	}

	a->array[a->len++] = value;
}
