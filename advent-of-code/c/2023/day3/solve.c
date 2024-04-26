#include <stdio.h>
#include <stdlib.h>
#include "read_file.h"
#include "parser.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 2;
	}

	//returns 0 terminated array
	int* parts = parser(filebuff);

	free(filebuff);

	int sum = 0;
	int* i = parts;

	while (*i != 0) {
		sum += *i++;
	}

	printf("%d\n", sum);

	free(parts);
	return 0;
}
