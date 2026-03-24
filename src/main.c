#include <stdio.h>
#include <string.h>

#include "file.h"
#include "progInfo.h"
#include "errorMake.h"

int main(
	int argc,
	char *argv[]
) {
	char buffer[LENGTH_BUFFER] = { 0 };
	
	FILE *fileInput;
	
	if (argc < 2) {
		fprintf(stderr, "ERROR: File wasn't specified\n");
		return 1;
	}
	
	if (file_open(
		&fileInput,
		argv[1],
		"r"
	)) {
		return 1;
	}	
	
	if (fgets(buffer, LENGTH_BUFFER, fileInput) == NULL) {
		fprintf(stderr, "ERROR: Failed to read file\n");
		return 1;
	}
	
	if (buffer[0] != LEGEND_OUTPUT) {
		fprintf(stderr, "ERROR: File is missing output(s)\n");
		return 1;
	}
	
	if (errorMake_getOutputs(buffer)) {
		return 1;
	}
	
	if (errorMake_readFile(fileInput)) {
		return 1;
	}
	
	fclose(fileInput);
	return 0;
}