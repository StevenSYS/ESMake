#define VARIABLE_NOEXTERNS
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "variable.h"
#include "progInfo.h"

#define STR_ERROR_REQUIRED_ARGS "ERROR: Required argument(s) is/are NULL\n"
#define STR_ERROR_FILE_READ "ERROR: Failed to read from file\n"
#define STR_ERROR_VAR_TYPE_UNKNOWN "ERROR: Unknown variable type: %s\n"
#define STR_ERROR_VAR_NAME "ERROR: Failed to get variable name\n"

#define SKIP_SPACE() \
	byte[0] = 0; \
	do { \
		if (!fread(byte, sizeof(char), 1, file)) { \
			fprintf(stderr, STR_ERROR_FILE_READ); \
			return 1; \
		} \
	} while ( \
		!byte[0] || \
		isspace(byte[0]) \
	); \
	fseek(file, -1, SEEK_CUR); \
	byte[0] = 0;

const char *variable_typeNames[VARTYPE_COUNT] = {
	"vo",
	"s8",
	"u8",
	"s16",
	"u16",
	"s32",
	"u32",
	"flt",
	"dou",
	"str",
	"obj",
	
	"au"
};

int variable_init(
	variable_t *var,
	size_t nameSize,
	size_t valSize
) {
	if (var == NULL) {
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
	
	if (object == NULL) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return;
	}
	
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
	VOARRAY_TYPE_SIZE i = 0;
	
	if (
		object == NULL ||
		var == NULL
	) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return NULL;
	}
	
	if (
		position != NULL &&
		*position < object->length
	) {
		i = *position;
	}
	
	for (; i < object->length; i++) {
		if (!object->i[i].used) {
			break;
		}
	}
	
	object->i[i].used = 1;
	object->i[i].type = var->type;
	object->i[i].name = var->name;
	object->i[i].value.ptr = var->value.ptr;
	
	object->length++;
	VOARRAY_RESIZE(variable_t, (*object), NULL);
	
	if (position != NULL) {
		*position = i;
	}
	return &object->i[i];
}

variable_t *variable_findObj(
	varObj_t *object,
	const char *name,
	VOARRAY_TYPE_SIZE *position
) {
	VOARRAY_TYPE_SIZE i = 0;
	
	if (
		object == NULL ||
		name == NULL
	) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return NULL;
	}
	
	if (
		position != NULL &&
		*position < object->length
	) {
		i = *position;
	}
	
	for (; i < (object->length - 1); i++) {
		if (strcmp(object->i[i].name, name) == 0) {
			return &object->i[i];
		}
	}
	
	if (position != NULL) {
		*position = i;
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

/* From File */
int variable_getFile(
	FILE *file,
	variable_t *var
) {
	char check = 0;
	char byte[2] = { 0, 0 };
	char buffer[LENGTH_BUFFER] = { 0 };
	size_t position;
	VOARRAY_TYPE_SIZE i = 0;
	
	if (
		file == NULL ||
		var == NULL
	) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return 1;
	}
	
	position = ftell(file);
	
	SKIP_SPACE();
	
	do {
		if (byte[0]) {
			buffer[i] = byte[0];
			i++;
		}
		
		if (!fread(byte, sizeof(char), 1, file)) {
			fprintf(stderr, STR_ERROR_FILE_READ);
			return 1;
		}
	} while (!isspace(byte[0]));
	
	for (i = 0; i < VARTYPE_COUNT; i++) {
		if (strncmp(
			buffer,
			variable_typeNames[i],
			LENGTH_BUFFER
		) == 0) {
			check = 1;
			var->type = (enum variable_types)i;
			break;
		}
	}
	
	if (!check) {
		fprintf(stderr, STR_ERROR_VAR_TYPE_UNKNOWN, buffer);
		return 1;
	}
	
	position = ftell(file);
	
	check = 0;
	
	name:
	i = 0;
	
	fseek(file, position, SEEK_SET);
	
	SKIP_SPACE();
	
	do {
		if (byte[0]) {
			if (check) {
				var->name[i] = byte[0];
			}
			i++;
		}
		
		if (!fread(byte, sizeof(char), 1, file)) {
			fprintf(stderr, STR_ERROR_FILE_READ);
			return 1;
		}
	} while (!isspace(byte[0]));
	
	if (!i) {
		fprintf(stderr, STR_ERROR_VAR_NAME);
		return 1;
	}
	
	if (!check) {
		var->name = malloc(
			sizeof(char[i])
		);
		
		check = 1;
		goto name;
	}
	return 0;
}