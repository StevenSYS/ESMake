#include <stdio.h>
#include <string.h>
#include <clonc_clon.h>

#include "file.h"
#include "progInfo.h"

int main(
	int argc,
	char *argv[]
) {
	int result = 0;
	char *fileStr;
	size_t fileLen;
	FILE *file;
	clonc_var_t var;
	
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
	fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	fileStr = malloc(fileLen);
	
	if (!fread(fileStr, sizeof(char), fileLen, file)) {
		fprintf(stderr, "ERROR: Failed to read file\n");
		return 1;
	}
	
	// getVar:
	result = clonc_getStr(
		fileStr,
		fileLen,
		&var
	);
	
	if (result > 0) {
		return 1;
	}
	
	if (result == 0) {
		/*printf(
			"Type: \"%s\"\n"
			"Name: \"%s\"\n",
			clonc_var_typeNames[var.type],
			var.name
		);*/
		
		/*if (var.type == VARTYPE_STR) {
			printf("Value: %s\n", var.value.str);
		} else {
			printf("Value: %zd\n", *var.value.sl);
		}*/
		
		clonc_uninit(&var);
		// goto getVar;
	}
	
	free(fileStr);
	fclose(file);
	return 0;
}