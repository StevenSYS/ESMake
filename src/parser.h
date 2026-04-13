#ifndef __PARSER__
#define __PARSER__

#include "VoArray/voArray.h"

/* VoArrays */
VOARRAY(vars_t, struct var_s);

/* Structs */
typedef struct var_s {
	char *name;
	char *value;
} var_t;

typedef struct {
	char *name;
	vars_t vars;
} section_t;

/* Functions */
section_t *parser_getSection(
	const char *str,
	size_t len,
	const char *name
);

var_t *parser_getVar(
	const section_t *sect,
	const char *name,
	size_t len
);

void parser_uninitVar(var_t *var);
void parser_uninitSection(section_t *sect);

#endif