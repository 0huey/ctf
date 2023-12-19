#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_file.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}

	char* filebuff = read_txt_file(argv[1]);

	if (filebuff == NULL) {
		return 1;
	}

	int calibration = 0;

	char* line = strtok(filebuff, "\n");

	while (line != NULL) {
		char tens;
		char ones;
		char* p = line;

		while (*p != '\0') {
			if (*p >= '0' && *p <= '9') {
				tens = *p - '0';
				break;
			}
			p++;
		}

		while(*p != '\0') {
			if (*p >= '0' && *p <= '9') {
				ones = *p - '0';
			}
			p++;
		}

		calibration += tens*10 + ones;

		line = strtok(NULL, "\n");
	}

	printf("%d\n", calibration);
	free(filebuff);
	return 0;
}
