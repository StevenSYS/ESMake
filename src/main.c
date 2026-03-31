#include <stdio.h>
#include <string.h>

#include "file.h"
#include "progInfo.h"
#include "errorMake.h"

extern char verbose_enabled;

int main(
	int argc,
	char *argv[]
) {
	FILE *fileInput;
	
	if (argc < 2) {
		fprintf(stderr, "ERROR: File wasn't specified\n");
		return 1;
	}
	
	if (
		argc > 2 &&
		strcmp(argv[2], ARGUMENT_VERBOSE) == 0
	) {
		verbose_enabled = 1;
	}
	
	if (file_open(
		&fileInput,
		argv[1],
		"r"
	)) {
		return 1;
	}
	
	if (errorMake_readFile(fileInput)) {
		return 1;
	}
	
	fclose(fileInput);
	return 0;
}