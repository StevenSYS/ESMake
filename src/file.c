#include <stdio.h>

int file_open(
	FILE **output,
	const char *filename,
	const char *mode
) {
	*output = fopen(filename, mode);
	
	if (*output == NULL) {
		perror("ERROR: Failed to open file");
		return 1;
	}
	return 0;
}