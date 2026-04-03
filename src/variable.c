#define VARIABLE_NOEXTERNS
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "varFlag.h"
#include "variable.h"
#include "progInfo.h"
#include "errorString.h"

#define FREADEC(_ptr, _size, _count, _file) \
	if (!fread(_ptr, _size, _count, _file)) { \
		fprintf(stderr, STR_ERROR_FILE_READ); \
		return 1; \
	}

#define SKIPSPACE() \
	byte = 0; \
	do { \
		byte = getc(file); \
		if ( \
			byte == EOF || \
			byte == VARIABLE_CHAR_STOP \
		) { \
			return -1; \
		} \
	} while ( \
		!byte || \
		isspace(byte) \
	); \
	fseek(file, -1, SEEK_CUR); \
	byte = 0;

const char *variable_typeNames[VARTYPE_COUNT] = {
	"vo",
	"num",
	"flt",
	"dou",
	"str",
	"obj"
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
	
	var->used = 1;
	
	if (nameSize) {
		var->name = malloc(nameSize);
	}
	
	if (valSize) {
		var->value.ptr = malloc(valSize);
	}
	return 0;
}

void variable_uninit(variable_t *var) {
	if (var == NULL) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return;
	}
	
	var->used = 0;
	var->type = VARTYPE_UNSET;
	
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
	
	if (
		!(var->flags & VARFLAG_BM(VARFLAG_AD)) &&
		variable_findObj(
			object,
			var->name,
			NULL
		) != NULL
	) {
		fprintf(stderr, STR_ERROR_VAR_EXISTS_OBJ);
		return NULL;
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
	char byte = 0;
	char check = 0;
	char buffer[LENGTH_BUFFER] = { 0 };
	enum varFlags flag = VARFLAG_NONE;
	size_t position;
	VOARRAY_TYPE_SIZE i;
	
	if (
		file == NULL ||
		var == NULL
	) {
		fprintf(stderr, STR_ERROR_REQUIRED_ARGS);
		return 1;
	}
	
	var->flags = 0;
	var->name = NULL;
	var->value.ptr = NULL;
	
	type:
	i = 0;
	position = ftell(file);
	
	SKIPSPACE();
	
	do {
		if (byte) {
			buffer[i] = byte;
			i++;
		}
		
		byte = getc(file);
	} while (
		byte &&
		!isspace(byte)
	);
	
	flag = varFlag_check(
		buffer,
		LENGTH_BUFFER
	);
	
	if (flag != VARFLAG_NONE) {
		if (!(var->flags & VARFLAG_BM(flag))) {
			var->flags += VARFLAG_BM(flag);
		}
		goto type;
	}
	
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
	
	if (!(var->flags & VARFLAG_BM(VARFLAG_NN))) {
		name:
		i = 0;
		
		fseek(file, position, SEEK_SET);
		
		SKIPSPACE();
		
		do {
			if (
				byte &&
				byte != VARIABLE_CHAR_END
			) {
				if (check) {
					var->name[i] = byte;
				}
				i++;
			}
			
			byte = getc(file);
		} while (
			byte &&
			!isspace(byte)
		);
		
		if (!i) {
			fprintf(stderr, STR_ERROR_VAR_NAME);
			return 1;
		}
		
		if (!check) {
			variable_init(
				var,
				sizeof(char[i]),
				0
			);
			
			check = 1;
			goto name;
		}
	}
	
	position = ftell(file);
	
	i = 0;
	check = 0;
	byte = 0;
	buffer[0] = 0;
	
	switch (var->type) {
		case VARTYPE_NUM:
			variable_init(
				var,
				0,
				sizeof(uint8_t)
			);
			
			do {
				if (byte) {
					buffer[i] = byte;
					i++;
				}
				
				byte = getc(file);
			} while (
				byte &&
				byte != VARIABLE_CHAR_END &&
				!isspace(byte)
			);
			
			if (byte != VARIABLE_CHAR_END) {
				fprintf(stderr, "§ERROR: Missing line end\n");
				return 1;
			}
			
			*var->value.num = (int32_t)atoi(buffer);
			break;
		case VARTYPE_STR:
			do {
				byte = getc(file);
			} while (
				byte &&
				byte != VARIABLE_CHAR_STR &&
				isspace(byte)
			);
			
			if (byte != VARIABLE_CHAR_STR) {
				fprintf(stderr, "§ERROR: Missing start of string\n");
				return 1;
			}
			
			position = ftell(file);
			
			do {
				if (
					byte &&
					byte != VARIABLE_CHAR_STR
				) {
					i++;
				}
				byte = getc(file);
			} while (
				byte &&
				byte != VARIABLE_CHAR_STR
			);
			
			if (byte != VARIABLE_CHAR_STR) {
				fprintf(stderr, "§ERROR: Missing end of string\n");
				return 1;
			}
			
			fseek(file, position, SEEK_SET);
			
			if (i) {
				variable_init(
					var,
					0,
					sizeof(char[i])
				);
				
				FREADEC(var->value.str, sizeof(char), i, file);
				var->value.str[i] = 0;
			}
			
			fseek(file, 1, SEEK_CUR);
			
			do {
				byte = getc(file);
			} while (
				byte &&
				byte != VARIABLE_CHAR_END &&
				isspace(byte)
			);
			
			if (byte != VARIABLE_CHAR_END) {
				fprintf(stderr, "§ERROR: Missing line end\n");
				return 1;
			}
			break;
		default:
			break;
	}
	return 0;
}