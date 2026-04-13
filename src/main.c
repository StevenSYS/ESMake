#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "parser.h"
#include "progInfo.h"

static const char *sectNames[3] = {
	"outputs",
	"variables",
	"items"
};

int main(
	int argc,
	char *argv[]
) {
	char *str = NULL;
	unsigned char j = 0;
	size_t i;
	size_t len;
	FILE *file;
	section_t *sect;
	
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
	
	for (j = 0; j < 3; j++) {
		sect = parser_getSection(
			str,
			len,
			sectNames[j]
		);
		
		if (sect == NULL) {
			return 1;
		}
		
		printf("- %s -\n", sect->name);
		for (i = 0; i < sect->vars.length; i++) {
			printf("%s", sect->vars.i[i].name);
			if (sect->vars.i[i].value != NULL) {
				printf(" = \"%s\"", sect->vars.i[i].value);
			}
			printf("\n");
		}
		
		for (i = 0; i < strlen(sect->name) + 4; i++) {
			printf("-");
		}
		printf("\n");
		
		parser_uninitSection(sect);
	}
	
	free(str);
	fclose(file);
	return 0;
}