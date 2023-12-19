struct int_array {
	int len;
	int max_len;
	int* array;
};

struct ptr_array {
	int len;
	int max_len;
	void* array;
};

void init_int_array(struct int_array* a);
void append_int_array(struct int_array* a, int value);
int int_comparator(const void* a, const void* b) ;

void init_ptr_array(struct ptr_array* a);
void append_ptr_array(struct ptr_array* a, void* value);
