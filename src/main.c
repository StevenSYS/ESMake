#include <stdlib.h>

#include "file.h"
#include "macros.h"
#include "esmake.h"

int main(
	int argc,
	char *argv[]
) {
	char *str = NULL;
	size_t len;
	FILE *file = NULL;
	
	if (argc < 2) {
		fprintf(stderr, "ERROR: File wasn't specified\n");
		return 1;
	}
	
	if (file_open(
		&file,
		argv[1],
		"r"
	)) {
		return 1;
	}
	
	fseek(file, 0, SEEK_END);
	len = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	MALLOC_EC(
		str,
		sizeof(char[len]),
		char *,
		1
	);
	fread(str, sizeof(char), len, file);
	fclose(file);
	
	esmake_process(str, len);
	
	free(str);
	return 0;
}