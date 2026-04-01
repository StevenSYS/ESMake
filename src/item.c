#include <ctype.h>
#include <string.h>

#include "item.h"
#include "verbose.h"
#include "progInfo.h"

enum types {
	TYPES_RET,
	TYPES_ENUM,
	
	TYPES_COUNT
};

enum vars {
	VARS_PREENUM,
	VARS_PRELANG,
	
	VARS_COUNT
};

static const char *typeNames[TYPES_COUNT] = {
	"ret",
	"enum"
};
static const char *varNames[VARS_COUNT] = {
	"preEnum",
	"preLang"
};

static FILE **fileList[TYPES_COUNT];
static char *varVal[VARS_COUNT];

void item_preInit() {
	unsigned char i;
	
	for (i = 0; i < TYPES_COUNT; i++) {
		fileList[i] = NULL;
	}
	
	for (i = 0; i < VARS_COUNT; i++) {
		varVal[i] = NULL;
	}
	return;
}

int item_init(
	files_t *files,
	variables_t *outVars,
	variables_t *vars
) {
	unsigned char i;
	VOARRAY_TYPE_SIZE position = 0;
	variable_t *tmpVar;
	static char fullInited = 0;
	
	if (fullInited) {
		VERBOSE_PRINTF("Skipping, already initialized\n");
		return 0;
	}
	
	fullInited = 1;
	
	for (i = 0; i < TYPES_COUNT; i++) {
		if (fileList[i] == NULL) {
			tmpVar = variable_find(
				outVars,
				typeNames[i],
				&position
			);
			
			if (tmpVar == NULL) {
				fullInited = 0;
			} else {
				fileList[i] = &files->i[position];
				fprintf(*fileList[i], AUTOGEN_TEXT);
			}
		}
	}
	
	for (i = 0; i < VARS_COUNT; i++) {
		if (varVal[i] == NULL) {
			tmpVar = variable_find(
				vars,
				varNames[i],
				NULL
			);
			
			if (tmpVar == NULL) {
				fullInited = 0;
			} else {
				varVal[i] = tmpVar->value;
			}
		}
	}
	return 0;
}

int item_cleanName(char *str) {
	size_t i;
	
	for (i = 0; str[i]; i++) {
		if (isspace(str[i])) {
			str[i] = 0;
			break;
		}
	}
	
	for (i++; str[i]; i++) {
		if (!isspace(str[i])) {
			fprintf(stderr, "ERROR: Non whitespace character after item name\n");
			return 1;
		}
	}
	return 0;
}

int item_add(
	const char *name,
	char def,
	size_t count
) {
	static size_t i = 0;
	
	if (def) {
		fprintf(
			*fileList[TYPES_RET],
			"default: return %s%s;\n",
			varVal[VARS_PRELANG],
			name
		);
	} else {
		fprintf(
			*fileList[TYPES_RET],
			"case %s%s: return %s%s;\n",
			varVal[VARS_PREENUM],
			name,
			varVal[VARS_PRELANG],
			name
		);
	}
	
	fprintf(
		*fileList[TYPES_ENUM],
		"%s%s",
		varVal[VARS_PREENUM],
		name
	);
	
	if (i < (count - 1)) {
		fprintf(*fileList[TYPES_ENUM], ",");
	}
	
	fprintf(*fileList[TYPES_ENUM], "\n");
	
	i++;
	return 0;
}