#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lang.h"
#include "type.h"
#include "varFlag.h"
#include "privInfo.h"

#include "clonc.h"

/* Macros */
#define BM(_val) (1 << _val)

#define FREAD_EC(_ptr, _size, _count, _file) \
	if (!fread(_ptr, _size, _count, _file)) { \
		fprintf(stderr, LANG_ERROR_FILE_READ); \
		return 1; \
	}

#define STR_EC(_str, _msg) \
	if ( \
		_str[i] && \
		!check \
	) { \
		fprintf(stderr, _msg); \
		return 1; \
	}

#define SKSP(_src, _str, _errMsg) \
	_str = _src + i; \
	\
	for (; _str[i]; i++) { \
		if (!isspace(_str[i])) { \
			break; \
		} \
	} \
	\
	check = 0; \
	\
	for (i++; _str[i]; i++) { \
		if (isspace(_str[i])) { \
			check = 1; \
			_str[i] = 0; \
			i++; \
			break; \
		} \
	} \
	\
	STR_EC(_str, _errMsg);

/* Static Functions */
static inline char *sgets(
	char **out,
	size_t len,
	const char *str
) {
	if (
		out == NULL ||
		str == NULL ||
		!len
	) {
		return NULL;
	}
	
	*out = (char *)memchr(str, '\n', len);
	
	if (*out != NULL) {
		*out[0] = 0;
		(*out)++;
	}
	return *out;
}

static inline char numCheck(
	const char *str,
	size_t len
) {
	char hasSpace = 0;
	size_t i;
	
	for (i = 0; str[i]; i++) {
		if (hasSpace) {
			if (!isspace(str[i])) {
				return 0;
			}
		} else {
			if (isspace(str[i])) {
				hasSpace = 1;
			} else if (
				str[i] < '0' ||
				str[i] > '9'
			) {
				return 0;
			}
		}
	}
	return 1;
}

/* Functions */

/* Variable */
int clonc_varInit(
	clonc_var_t *var,
	size_t nameSize,
	size_t valSize
) {
	if (var == NULL) {
		fprintf(stderr, LANG_ERROR_REQUIRED_ARGS);
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

int clonc_varUninit(clonc_var_t *var) {
	if (var == NULL) {
		fprintf(stderr, LANG_ERROR_REQUIRED_ARGS);
		return 1;
	}
	
	var->used = 0;
	var->type = (unsigned char)TYPE_UNKNOWN;
	
	if (var->name != NULL) {
		free(var->name);
		var->name = NULL;
	}
	
	if (var->value.ptr != NULL) {
		free(var->value.ptr);
		var->value.ptr = NULL;
	}
	return 0;
}

/* Object */
void clonc_objClear(clonc_obj_t *obj) {
	VOARRAY_TYPE_SIZE i;
	
	if (obj == NULL) {
		fprintf(stderr, LANG_ERROR_REQUIRED_ARGS);
		return;
	}
	
	for (i = 0; i < obj->length; i++) {
		obj->i[i].used = 0;
		obj->i[i].type = TYPE_VO;
		obj->i[i].name = NULL;
		obj->i[i].value.ptr = NULL;
	}
	return;
}

int clonc_objUninit(clonc_obj_t *object) {
	VOARRAY_TYPE_SIZE i;
	
	for (i = 0; i < (object->length - 1); i++) {
		if (object->i[i].type == TYPE_OBJ) {
			if (clonc_objUninit(object->i[i].value.obj)) {
				return 1;
			}
		}
		
		if (clonc_varUninit(&object->i[i])) {
			return 1;
		}
	}
	VOARRAY_UNINIT((*object));
	return 0;
}

/* Object - Variable */
clonc_var_t *clonc_objAddVar(
	clonc_obj_t *object,
	clonc_var_t *var,
	VOARRAY_TYPE_SIZE *position
) {
	VOARRAY_TYPE_SIZE i = 0;
	
	if (
		object == NULL ||
		var == NULL
	) {
		fprintf(stderr, LANG_ERROR_REQUIRED_ARGS);
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
		!(var->flags & BM(VARFLAG_AD)) &&
		clonc_objFindVar(
			object,
			var->name,
			NULL
		) != NULL
	) {
		fprintf(stderr, LANG_ERROR_VAR_EXISTS_OBJ);
		return NULL;
	}
	
	object->i[i].used = 1;
	object->i[i].type = var->type;
	object->i[i].name = var->name;
	object->i[i].value.ptr = var->value.ptr;
	
	object->length++;
	VOARRAY_RESIZE(clonc_var_t, (*object), NULL);
	
	if (position != NULL) {
		*position = i;
	}
	return &object->i[i];
}

clonc_var_t *clonc_objFindVar(
	clonc_obj_t *obj,
	const char *name,
	VOARRAY_TYPE_SIZE *position
) {
	VOARRAY_TYPE_SIZE i = 0;
	
	if (
		obj == NULL ||
		name == NULL
	) {
		fprintf(stderr, LANG_ERROR_REQUIRED_ARGS);
		return NULL;
	}
	
	if (
		position != NULL &&
		*position < obj->length
	) {
		i = *position;
	}
	
	for (; i < (obj->length - 1); i++) {
		if (strcmp(obj->i[i].name, name) == 0) {
			return &obj->i[i];
		}
	}
	
	if (position != NULL) {
		*position = i;
	}
	return NULL;
}

/* Object - String */
int clonc_objFromStr(
	const char *str,
	size_t len,
	clonc_obj_t *obj
) {
	char *strCopy = (char *)malloc(sizeof(char[len]));
	char check = 0;
	char *prev = NULL;
	char *current = strCopy;
	char *tmpStr = NULL;
	size_t i = 0;
	size_t lineLen = 0;
	clonc_var_t *var;
	
	strncpy(strCopy, str, len);
	
	obj->length = 1;
	VOARRAY_INIT(clonc_var_t, (*obj), 1);
	obj->i[0].used = 0;
	
	while (current != NULL) {
		var = &obj->i[obj->length - 1];
		obj->length++;
		VOARRAY_RESIZE(clonc_var_t, (*obj), 1);
		var->used = 1;
		i = 0;
		
		prev = current;
		sgets(&current, len, strCopy);
		
		SKSP(prev, tmpStr, LANG_ERROR_VAR_GET_TYPE);
		
		var->type = (unsigned char)type_get(tmpStr, strlen(tmpStr));
		
		if (var->type == TYPE_UNKNOWN) {
			fprintf(stderr, LANG_ERROR_VAR_TYPE_UNKNOWN, tmpStr);
			return 1;
		}
		
		SKSP(tmpStr, tmpStr, LANG_ERROR_VAR_GET_NAME);
		
		tmpStr = tmpStr + i;
		STR_EC(tmpStr, LANG_ERROR_VAR_GET_VALUE);
		
		
		switch (var->type) {
			case TYPE_IN:
				/*var->value.in = malloc(sizeof(int));
				if (!numCheck(tmpStr, lineLen)) {
					fprintf(stderr, LANG_ERROR_VAR_INVALID_VALUE);
					return 1;
				}
				*var->value.in = atoi(tmpStr);*/
				break;
			case TYPE_STR:
				// var->value.str = malloc(sizeof(char[lineLen]));
				break;
			default:
				break;
		}
	}
	
	free(strCopy);
	return 0;
}