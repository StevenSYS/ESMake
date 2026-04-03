#include <stdio.h>
#include <string.h>

#include "file.h"
#include "variable.h"
#include "progInfo.h"

int main(
	int argc,
	char *argv[]
) {
	int result = 0;
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
	
	getVar:
	result = variable_getFile(file, &var);
	
	if (result > 0) {
		return 1;
	}
	
	if (result == 0) {
		printf(
			"Type: \"%s\"\n"
			"Name: \"%s\"\n",
			variable_typeNames[var.type],
			var.name
		);
		
		if (var.type == VARTYPE_STR) {
			printf("Value: %s\n", var.value.str);
		} else {
			printf("Value: %d\n", *var.value.num);
		}
		
		variable_uninit(&var);
		goto getVar;
	}
	
	fclose(file);
	return 0;
}