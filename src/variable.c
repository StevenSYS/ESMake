#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "verbose.h"
#include "variable.h"

int variable_add(
	variables_t *vars,
	const variable_t *var
) {
	VERBOSE_PRINTF("%s - Adding variable to variable list\n", var->name);
	strncpy(
		vars->i[vars->length - 1].name,
		var->name,
		VARIABLE_LENGTH_STRING
	);
	strncpy(
		vars->i[vars->length - 1].value,
		var->value,
		VARIABLE_LENGTH_STRING
	);
	VERBOSE_PRINTF("%s - Added variable to variable list\n", var->name);
	
	VERBOSE_PRINTF("%s - Increasing variable list size\n", var->name);
	vars->length++;
	VOARRAY_RESIZE(variable_t, (*vars), 1);
	VERBOSE_PRINTF("%s - Increased variable list size: %zu\n", var->name, vars->length);
	return 0;
}

variable_t *variable_find(
	variables_t *variables,
	const char *find,
	VOARRAY_TYPE_SIZE *position
) {
	VOARRAY_TYPE_SIZE i;
	
	VERBOSE_PRINTF("%s - Searching for variable in variable list\n", find);
	
	for (i = 0; i < variables->length; i++) {
		if (strncmp(
			variables->i[i].name,
			find,
			VARIABLE_LENGTH_STRING
		) == 0) {
			VERBOSE_PRINTF("%s - Found variable in variable list\n", find);
			if (position != NULL) {
				*position = i;
			}
			return &variables->i[i];
		}
	}
	
	VERBOSE_PRINTF("%s - Failed to find variable in variable list\n", find);
	return NULL;
}

int variable_getStr(
	variables_t *variables,
	char *str,
	VOARRAY_TYPE_SIZE *position
) {
	char check = 0;
	char *value;
	unsigned char j;
	size_t i;
	
	variable_t tmpVar;
	variable_t *tmpVarPtr;
	
	for (i = 1; str[i]; i++) {
		if (isspace(str[i])) {
			str[i] = 0;
			strncpy(
				tmpVar.name,
				str + 1,
				VARIABLE_LENGTH_STRING
			);
			break;
		}
	}
	
	for (j = 0; j < 2; j++) {
		check = 0;
		
		for (i++; str[i]; i++) {
			if (str[i] == '"') {
				if (j == 0) {
					value = str + (i + 1);
				} else {
					check = 1;
					str[i] = 0;
					strncpy(
						tmpVar.value,
						value,
						VARIABLE_LENGTH_STRING
					);
				}
				break;
			} else if (
				j == 0 &&
				!isspace(str[i])
			) {
				fprintf(stderr, "ERROR: Non whitespace character before value string\n");
				return 1;
			}
		}
		
		if (
			j != 0 &&
			!check
		) {
			fprintf(stderr, "ERROR: Missing end of value string\n");
			return 1;
		}
	}
	
	tmpVarPtr = variable_find(
		variables,
		tmpVar.name,
		position
	);
	
	if (tmpVarPtr == NULL) {
		if (position != NULL) {
			*position = variables->length - 1;
		}
		variable_add(variables, &tmpVar);
	} else {
		strncpy(
			tmpVarPtr->value,
			tmpVar.value,
			VARIABLE_LENGTH_STRING
		);
	}
	return 0;
}