#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variable.h"

#define STR_ERROR_REQUIRED_ARGS "ERROR: Required argument(s) is/are NULL\n"

int variable_init(
	variable_t *var,
	size_t nameSize,
	size_t valSize
) {
	if (var = NULL) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return 1;
	}
	
	if (nameSize) {
		var->name = malloc(nameSize);
	} else {
		var->name = NULL;
	}
	
	if (valSize) {
		var->value.ptr = malloc(valSize);
	} else {
		var->value.ptr = NULL;
	}
	return 0;
}

void variable_uninit(variable_t *var) {
	if (var == NULL) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return;
	}
	
	var->used = 0;
	var->type = VARTYPE_VO;
	
	if (var->name != NULL) {
		free(var->name);
		var->name = NULL;
	}
	
	if (var->value.ptr != NULL) {
		free(var->value.ptr);
		var->value.ptr = NULL;
	}
	return;
}

/* Object */
void variable_clearObj(varObj_t *object) {
	VOARRAY_TYPE_SIZE i;
	
	for (i = 0; i < object->length; i++) {
		object->i[i].used = 0;
		object->i[i].type = VARTYPE_VO;
		object->i[i].name = NULL;
		object->i[i].value.ptr = NULL;
	}
	return;
}

variable_t *variable_addObj(
	varObj_t *object,
	variable_t *var,
	VOARRAY_TYPE_SIZE *position
) {
	if (
		object == NULL ||
		var == NULL ||
		position == NULL
	) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return NULL;
	}
	
	for (*position = 0; *position < object->length; *position++) {
		if (!object->i[*position].used) {
			break;
		}
	}
	
	object->i[*position].used = 1;
	object->i[*position].type = var->type;
	object->i[*position].name = var->name;
	object->i[*position].value.ptr = var->value.ptr;
	
	object->length++;
	VOARRAY_RESIZE(variable_t, (*object), NULL);
	return &object->i[*position];
}

variable_t *variable_findObj(
	varObj_t *object,
	const char *name,
	VOARRAY_TYPE_SIZE *position
) {
	if (
		object == NULL ||
		name == NULL ||
		position == NULL
	) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return NULL;
	}
	
	if (*position > (object->length - 1)) {
		*position = 0;
	}
	
	for (; *position < (object->length - 1); *position++) {
		if (strcmp(object->i[*position].name, name) == 0) {
			return &object->i[*position];
		}
	}
	return NULL;
}

int variable_uninitObj(varObj_t *object) {
	VOARRAY_TYPE_SIZE i;
	
	for (i = 0; i < (object->length - 1); i++) {
		if (object->i[i].type == VARTYPE_OBJ) {
			if (variable_uninitObj(object->i[i].value.obj)) {
				return 1;
			}
		}
		variable_uninit(&object->i[i]);
	}
	VOARRAY_UNINIT((*object));
	return 0;
}