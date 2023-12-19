#include <stdlib.h>
#include <string.h>
#include "my_array.h"


#ifndef MY_ARRAY_LEN
#define MY_ARRAY_LEN 20
#endif

void init_int_array(struct int_array* a) {
	a->len = 0;
	a->max_len = MY_ARRAY_LEN;
	a->array = calloc(sizeof(int), a->max_len);
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
	a->array = calloc(sizeof(void*), a->max_len);
}

void append_ptr_array(struct ptr_array* a, void* value) {
	if (a->len == a->max_len) {
		a->max_len += MY_ARRAY_LEN;
		void* tmp = calloc(sizeof(void*), a->max_len);
		memcpy(tmp, a->array, sizeof(void*) * a->len);
		free(a->array);
		a->array = tmp;
	}

	a->array[a->len++] = value;
}
