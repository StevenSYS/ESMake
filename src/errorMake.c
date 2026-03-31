#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "file.h"
#include "item.h"
#include "verbose.h"
#include "variable.h"
#include "progInfo.h"

// static char check = 0;

static inline size_t getItemCount(FILE *fileInput) {
	char buffer[LENGTH_BUFFER] = { 0 };
	size_t ret = 0;
	long int prevPos = ftell(fileInput);
	
	VERBOSE_PRINTF("Getting item count...\n");
	
	while (fgets(buffer, LENGTH_BUFFER, fileInput) != NULL) {
		if (
			buffer[0] == LEGEND_ITEM ||
			buffer[0] == LEGEND_ITEM_DEFAULT
		) {
			ret++;
		}
	}
	
	VERBOSE_PRINTF("Got item count: %zu\n", ret);
	
	fseek(fileInput, prevPos, SEEK_SET);
	return ret;
}

int errorMake_readFile(FILE *fileInput) {
	char buffer[LENGTH_BUFFER] = { 0 };
	unsigned char offset;
	VOARRAY_TYPE_SIZE i;
	size_t bufLen;
	size_t itemCount = getItemCount(fileInput);
	files_t files = { NULL, 1 };
	variables_t outVars = { NULL, 1 };
	variables_t vars = { NULL, 1 };
	
	if (fileInput == NULL) {
		fprintf(stderr, "ERROR: Required argument is NULL\n");
		return 1;
	}
	
	VOARRAY_INIT(FILE *, files, 1);
	VOARRAY_INIT(variable_t, vars, 1);
	VOARRAY_INIT(variable_t, outVars, 1);
	
	while (fgets(buffer, LENGTH_BUFFER, fileInput) != NULL) {
		offset = 0;
		
		if (
			(
				buffer[0] != LEGEND_OUTPUT &&
				buffer[0] != LEGEND_VARIABLE &&
				buffer[0] != LEGEND_COMMENT
			) &&
			!files.length
		) {
			fprintf(stderr, "ERROR: No output files have been specified\n");
			return 1;
		}
		
		switch (buffer[0]) {
			case LEGEND_OUTPUT:
				if (variable_getStr(&outVars, buffer)) {
					return 1;
				}
				file_open(
					&files.i[files.length - 1],
					outVars.i[outVars.length - 2].value,
					"w"
				);
				VERBOSE_PRINTF("Increasing file list size...\n");
				files.length++;
				VOARRAY_RESIZE(FILE *, files, 1);
				
				VERBOSE_PRINTF("Increased file list size\n");
				break;
			case LEGEND_VARIABLE:
				if (variable_getStr(&vars, buffer)) {
					return 1;
				}
				break;
			case LEGEND_ITEM:
			case LEGEND_ITEM_DEFAULT:
				if (item_addFile(
					buffer,
					&files,
					&outVars,
					&vars
				)) {
					return 1;
				}
				break;
			case LEGEND_ITEM_OUT:
				break;
			case LEGEND_COMMENT:
				break;
			case LEGEND_ESCAPE:
				offset = 1;
			default:
				bufLen = strlen(buffer + offset);
				for (i = 0; i < files.length - 1; i++) {
					fwrite(
						buffer + offset,
						sizeof(char),
						bufLen,
						files.i[i]
					);
				}
				break;
		}
	}
	
	VOARRAY_UNINIT(outVars);
	VOARRAY_UNINIT(vars);
	VOARRAY_UNINIT(files);
	return 0;
}