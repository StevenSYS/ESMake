#include <stdio.h>

int file_open(
	FILE **output,
	const char *filename,
	const char *mode
) {
	if (output == NULL) {
		fprintf(stderr, "ERROR: Required argument is NULL\n");
		return 1;
	}
	
	if (*output != NULL) {
		fclose(*output);
	}
	
	*output = fopen(filename, mode);
	
	if (*output == NULL) {
		perror("ERROR: Failed to open file");
		return 1;
	}
	return 0;
}