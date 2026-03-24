#include <stdio.h>
#include <string.h>

#include "file.h"
#include "item.h"
#include "setting.h"
#include "progInfo.h"
#include "whitespace.h"

static char check = 0;
static char *outNames[MAX_OUTPUTS];

static inline unsigned int getItemCount(FILE *fileInput) {
	char buffer[LENGTH_BUFFER] = { 0 };
	unsigned int ret = 0;
	long int prevPos = ftell(fileInput);
	
	while (fgets(buffer, LENGTH_BUFFER, fileInput) != NULL) {
		if (
			buffer[0] == LEGEND_ITEM ||
			buffer[0] == LEGEND_ITEMDEFAULT
		) {
			ret++;
		}
	}
	
	fseek(fileInput, prevPos, SEEK_SET);
	return ret;
}

static inline unsigned int initFiles(FILE *files[]) {
	unsigned int i;
	unsigned int ret = 0;
	
	for (i = 0; outNames[i] != NULL && i < MAX_OUTPUTS; i++) {
		if (file_open(
			&files[i],
			outNames[i],
			"w"
		)) {
			return 0;
		}
		
		fprintf(files[i], AUTOGEN_TEXT);
		ret++;
	}
	
	if (!ret) {
		fprintf(stderr, "ERROR: All files are NULL\n");
		return 0;
	}
	return ret;
}

int errorMake_getOutputs(char *buffer) {
	unsigned char j;
	unsigned int i = 0;
	unsigned int k;
	
	for (k = 0; k < MAX_OUTPUTS; k++) {
		outNames[k] = NULL;
	}
	
	for (k = 0; k < MAX_OUTPUTS; k++) {
		for (j = 0; j < 2; j++) {
			check = 0;
			
			for (i++; buffer[i]; i++) {
				if (buffer[i] == '"') {
					check = 1;
					if (j == 0) {
						outNames[k] = buffer + (i + 1);
					} else {
						buffer[i] = 0;
					}
					break;
				} else if (
					j == 0 &&
					!whitespace(buffer[i])
				) {
					fprintf(stderr, "ERROR: Output contains non whitespace character\n");
					return 1;
				}
			}
			
			if (!check) {
				if (
					j == 0 &&
					k > 0
				) {
					break;
				} else if (j != 0) {
					fprintf(stderr, "ERROR: Output is missing end of string\n");
				}
				return 1;
			}
		}
	}
	return 0;
}

int errorMake_readFile(FILE *fileInput) {
	char buffer[LENGTH_BUFFER] = { 0 };
	unsigned int i;
	unsigned int fileCount = 0;
	unsigned int offset = 0;
	unsigned int itemCount = getItemCount(fileInput);
	size_t bufLen;
	enum modes mode;
	FILE *files[MAX_OUTPUTS];
	settings_t settings = { 0 };
	setting_t *tmpSet;
	
	if (fileInput == NULL) {
		fprintf(stderr, "ERROR: Required argument is NULL\n");
		return 1;
	}
	
	fileCount = initFiles(files);
	
	if (!fileCount) {
		return 1;
	}
	
	while (fgets(buffer, LENGTH_BUFFER, fileInput) != NULL) {
		offset = 0;
		
		switch (buffer[0]) {
			case LEGEND_OUTPUT:
				fprintf(stderr, "ERROR: Output file(s) can only be specified at the beginning of the file\n");
				return 1;
			case LEGEND_SETTING:
				if (setting_getStr(&settings, buffer)) {
					return 1;
				}
				break;
			case LEGEND_ITEM:
			case LEGEND_ITEMDEFAULT:
				tmpSet = setting_find(&settings, SETTING_MODE);
			
				if (tmpSet == NULL) {
					fprintf(stderr, "ERROR: \"" SETTING_MODE "\" isn't set\n");
					return 1;
				}
				
				if (mode_getStr(
					tmpSet->value,
					&mode
				)) {
					return 1;
				}
				
				if (item_addFile(
					files,
					fileCount,
					itemCount,
					buffer,
					mode,
					&settings
				)) {
					return 1;
				}
				break;
			case LEGEND_COMMENT:
				break;
			case LEGEND_ESCAPE:
				offset = 1;
			default:
				bufLen = strlen(buffer + offset);
				for (i = 0; i < fileCount; i++) {
					fwrite(
						buffer + offset,
						sizeof(char),
						bufLen,
						files[i]
					);
				}
				break;
		}
	}
	
	for (i = 0; i < fileCount; i++) {
		fclose(files[i]);
	}
	return 0;
}