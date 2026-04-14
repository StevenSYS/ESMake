#include <stdio.h>
#include <string.h>

#include "file.h"
#include "parser.h"
#include "macros.h"
#include "progInfo.h"

#define REQCHECK( \
	_count, \
	_nameArr, \
	_posArr \
) \
	if (count < REQOUTS_COUNT) { \
		for (k = 0; k < REQOUTS_COUNT; k++) { \
			tmpLen = strlen(_nameArr[k]); \
			if ( \
				strlen(NAME) == tmpLen && \
				strncmp(NAME, _nameArr[k], tmpLen) == 0 \
			) { \
				_posArr[k] = i; \
				count++; \
				break; \
			} \
		} \
	}

/* Sections */
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

/* Outputs */
enum reqOut {
	REQOUTS_RET,
	REQOUTS_ENUM,
	
	REQOUTS_COUNT
};
static const char *reqOutNames[REQOUTS_COUNT] = { "ret", "enum" };
static size_t reqOutPos[REQOUTS_COUNT];

/* Variables */
enum reqVars {
	REQVARS_PREENUM,
	REQVARS_PRELANG,
	
	REQVARS_COUNT
};
static const char *reqVarNames[REQVARS_COUNT] = { "preEnum", "preLang" };
static size_t reqVarPos[REQVARS_COUNT];

int esmake_process(
	const char *str,
	size_t len
) {
	char check = 0;
	char def = 0;
	char *itemName = NULL;
	unsigned char j, l;
	size_t i, k;
	size_t count = 0;
	size_t tmpLen = 0;
	FILE **outFiles = NULL;
	section_t *sects[SECTION_COUNT];
	
	#define SECT sects[j]
	#define SIZE SECT->vars.size
	#define NAME SECT->vars.i[i].name
	#define VALUE SECT->vars.i[i].value
	
	#define REQVAR(_val) sects[SECTION_VARIABLES]->vars.i[REQVARS_ ## _val].value
	#define REQOUT(_val) outFiles[reqVarPos[REQOUTS_ ## _val]]
	
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
					check = 0;
					
					MALLOC_EC(
						outFiles,
						sizeof(FILE *[SIZE]),
						FILE **,
						1
					);
					
					for (i = 0; i < SIZE; i++) {
						REQCHECK(REQOUTS_COUNT, reqOutNames, reqOutPos);
						
						if (VALUE == NULL) {
							fprintf(
								stderr,
								"ERROR: Required output is missing value: %s\n",
								NAME
							);
							return 1;
						}
						
						parser_getVar(
							SECT,
							NAME,
							strlen(NAME),
							i + 1,
							&check
						);
						
						if (!check) {
							fprintf(
								stderr,
								"ERROR: Output variable already exists: %s\n",
								NAME
							);
							return 1;
						}
						
						outFiles[i] = NULL;
						
						if (file_open(
							&outFiles[i],
							VALUE,
							"w"
						)) {
							return 1;
						}
						
						fprintf(outFiles[i], AUTOGEN_TEXT);
					}
				}
				
				if (count < REQOUTS_COUNT) {
					fprintf(
						stderr,
						"ERROR: Missing %zu required output(s)\n",
						REQOUTS_COUNT - count
					);
					return 1;
				}
				break;
			case SECTION_VARIABLES:
				count = 0;
				
				for (i = 0; i < SIZE; i++) {
					if (VALUE == NULL) {
						fprintf(
							stderr,
							"ERROR: Required variable is missing value: %s\n",
							NAME
						);
						return 1;
					}
					REQCHECK(REQVARS_COUNT, reqVarNames, reqVarPos);
				}
				
				if (count < REQOUTS_COUNT) {
					fprintf(
						stderr,
						"ERROR: Missing %zu required variable(s)\n",
						REQVARS_COUNT - count
					);
					return 1;
				}
				break;
			case SECTION_ITEMS:
				count = 0;
				tmpLen = 0;
				for (l = 0; l < 2; l++) {
					for (i = 0; i < SIZE; i++) {
						if (VALUE == NULL) {
							if (!def) {
								def = (char)(strcmp(NAME, VARIABLE_DEFAULT) == 0);
								if (def) {
									continue;
								}
							}
							
							if (l) {
								itemName = NAME;
								
								/* Enum */
								fprintf(
									REQOUT(ENUM),
									"%s%s",
									REQVAR(PREENUM),
									itemName
								);
								
								if (tmpLen < count - 1) {
									fprintf(REQOUT(ENUM), ",");
								}
								
								fprintf(REQOUT(ENUM), "\n");
								
								/* Ret */
								if (def) {
									fprintf(REQOUT(RET), "default");
								} else {
									fprintf(
										REQOUT(RET),
										"case %s%s",
										REQVAR(PREENUM),
										itemName
									);
								}
								
								fprintf(
									REQOUT(RET),
									": return %s%s;\n",
									REQVAR(PRELANG),
									itemName
								);
								
								tmpLen++;
							} else {
								count++;
							}
							def = 0;
						} else if (l) {
							k = parser_getVar(
								sects[SECTION_OUTPUTS],
								NAME,
								strlen(NAME),
								0,
								&check
							);
							
							if (check) {
								fprintf(stderr, "ERROR: Failed to get file: %s\n", NAME);
								break;
							} else {
								fprintf(
									outFiles[k],
									"#define %s%s \"%s\"\n",
									REQVAR(PRELANG),
									itemName,
									VALUE
								);
							}
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
	
	for (i = 0; i < SECTION_COUNT; i++) {
		parser_uninitSection(sects[i]);
	}
	
	free(outFiles);
	
	#undef SECT
	#undef SIZE
	#undef NAME
	#undef VALUE
	
	#undef REQVAR
	#undef REQOUT
	return 0;
}