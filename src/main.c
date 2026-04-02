#include <stdio.h>
#include <string.h>

#include "file.h"
#include "variable.h"
#include "progInfo.h"

int main(
	int argc,
	char *argv[]
) {
	FILE *file;
	variable_t var;
	
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
	
	if (variable_getFile(file, &var)) {
		return 1;
	}
	
	printf(
		"Type: \"%s\"\n"
		"Name: \"%s\"\n",
		variable_typeNames[var.type],
		var.name
	);
	
	/*if (errorMake_readFile(file)) {
		return 1;
	}*/
	
	variable_uninit(&var);
	
	fclose(file);
	return 0;
}