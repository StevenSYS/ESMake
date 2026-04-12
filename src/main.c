#include <stdio.h>
#include <clonc.h>
#include <string.h>

#include "file.h"
#include "progInfo.h"

int main(
	int argc,
	char *argv[]
) {
	char *fileStr;
	size_t fileLen;
	FILE *file;
	clonc_obj_t obj;
	
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
	
	if (clonc_objFromStr(
		fileStr,
		fileLen,
		&obj
	)) {
		return 1;
	}
	
	/*if (clonc_objUninit(&obj)) {
		return 1;
	}*/
	
	VOARRAY_UNINIT(obj);
	free(fileStr);
	fclose(file);
	return 0;
}