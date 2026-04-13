#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "parser.h"
#include "progInfo.h"

enum sections {
	SECTION_OUTPUTS,
	SECTION_VARIABLES,
	SECTION_ITEMS,
	
	SECTION_COUNT
};

static const char *sectNames[SECTION_COUNT] = {
	"outputs",
	"variables",
	"items"
};

static section_t *sects[SECTION_COUNT];

int main(
	int argc,
	char *argv[]
) {
	char check = 0;
	char *str = NULL;
	char *name = NULL;
	char *value = NULL;
	unsigned char j = 0;
	size_t i, k;
	size_t len;
	FILE *file = NULL;
	FILE **outFiles = NULL;
	
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
	
	str = (char *)malloc(sizeof(char[len]));
	fread(str, sizeof(char), len, file);
	fclose(file);
	
	for (j = 0; j < SECTION_COUNT; j++) {
		sects[j] = parser_getSection(
			str,
			len,
			sectNames[j]
		);
		
		if (sects[j] == NULL) {
			return 1;
		}
		
		switch (j) {
			case SECTION_OUTPUTS:
				if (outFiles == NULL) {
					outFiles = (FILE **)malloc(
						sizeof(
							FILE *[sects[j]->vars.size]
						)
					);
					
					for (i = 0; i < sects[j]->vars.size; i++) {
						parser_getVar(
							sects[j],
							sects[j]->vars.i[i].name,
							strlen(sects[j]->vars.i[i].name),
							i + 1,
							&check
						);
						
						if (!check) {
							fprintf(stderr, "ERROR: Output variable already exists\n");
							return 1;
						}
						
						outFiles[i] = NULL;
						
						if (file_open(
							&outFiles[i],
							sects[j]->vars.i[i].value,
							"w"
						)) {
							return 1;
						}
					}
				}
				break;
			case SECTION_ITEMS:
				for (i = 0; i < sects[j]->vars.size; i++) {
					name = sects[j]->vars.i[i].name;
					value = sects[j]->vars.i[i].value;
					
					if (value != NULL) {
						k = parser_getVar(
							sects[SECTION_OUTPUTS],
							name,
							strlen(name),
							0,
							&check
						);
						
						if (check) {
							fprintf(stderr, "ERROR: Failed to get file: %s\n", name);
							break;
						} else {
							printf("%s %s\n", name, value);
						}
					}
				}
				break;
			default:
				break;
		}
		
	}
	
	for (i = 0; i < sects[SECTION_OUTPUTS]->vars.size; i++) {
		fclose(outFiles[i]);
	}
	
	for (j = 0; j < SECTION_COUNT; j++) {
		parser_uninitSection(sects[j]);
	}
	
	free(outFiles);
	free(str);
	return 0;
}