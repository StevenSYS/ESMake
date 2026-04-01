#ifndef __VARIABLE__
#define __VARIABLE__

#include "VoArray/voArray.h"

/* Macros */
#define VARIABLE_LENGTH 10
#define VARIABLE_LENGTH_STRING 100

/* Structs */
typedef struct {
	char name[VARIABLE_LENGTH_STRING];
	char value[VARIABLE_LENGTH_STRING];
} variable_t;

/* VoArrays */
VOARRAY(variables_t, variable_t);

/* Functions */
int variable_add(
	variables_t *vars,
	const variable_t *var
);

variable_t *variable_find(
	variables_t *vars,
	const char *find,
	VOARRAY_TYPE_SIZE *position
);

int variable_getStr(
	variables_t *vars,
	char *str,
	VOARRAY_TYPE_SIZE *position
);

#endif