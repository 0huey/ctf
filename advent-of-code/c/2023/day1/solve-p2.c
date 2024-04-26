#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "read_file.h"

const char* numbers[10] = {
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

const size_t numbers_len[10] = {4,3,3,5,4,4,3,5,5,4};

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

		// find 10's place
		while (*p != '\0') {
			if (*p >= '0' && *p <= '9') {
				tens = *p - '0';
				break;
			}

			bool numfound = false;

			for (char i = 0; i < 10; i++) {
				if (strncmp(p, numbers[i], numbers_len[i]) == 0) {
					tens = i;
					numfound = true;
					break;
				}
			}
			if (numfound) {
				break;
			}

			p++;
		}

		// find 1's place
		while(*p != '\0') {
			if (*p >= '0' && *p <= '9') {
				ones = *p - '0';
			}

			for (char i = 0; i < 10; i++) {
				if (strncmp(p, numbers[i], numbers_len[i]) == 0) {
					ones = i;
				}
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
