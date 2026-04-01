#include <stdio.h>

#include "verbose.h"

int file_open(
	FILE **output,
	const char *filename,
	const char *mode
) {
	if (*output != NULL) {
		VERBOSE_PRINTF("Output pointer had an already open file, closing\n");
		fclose(*output);
	}
	
	*output = fopen(filename, mode);
	
	VERBOSE_PRINTF("%s (%s) - Opening file\n", filename, mode);
	
	if (*output == NULL) {
		perror("ERROR: Failed to open file");
		return 1;
	}
	
	VERBOSE_PRINTF("%s (%s) - Opened file\n", filename, mode);
	return 0;
}