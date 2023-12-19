#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char* read_txt_file(char* filename) {
	if (filename == NULL) {
		return NULL;
	}

	struct stat filestat;

	if(stat(filename, &filestat) == -1) {
		return NULL;
	}

	int fd = open(filename, O_RDONLY);

	if (fd == -1) {
		return NULL;
	}

	char* filebuff = malloc(filestat.st_size + 1); // add 1 to append null term

	if (read(fd, filebuff, filestat.st_size) == -1) {
		free(filebuff);
		close(fd);
		return NULL;
	}

	filebuff[filestat.st_size] = '\0';

	close(fd);

	return filebuff;
}
