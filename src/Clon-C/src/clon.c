#define CLONC_NOEXTERNS
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lang.h"
#include "varFlag.h"
#include "privInfo.h"

#include "clonc_clon.h"

#define FREADEC(_ptr, _size, _count, _file) \
	if (!fread(_ptr, _size, _count, _file)) { \
		fprintf(stderr, LANG_ERROR_FILE_READ); \
		return 1; \
	}

const char *clonc_typeNames[CLONC_TYPE_COUNT] = {
	"vo",
	"u8",
	"s8",
	#if CLONC_BIT >= 16
	"u16",
	"s16",
	#if CLONC_BIT >= 32
	"u32",
	"s32",
	#if CLONC_BIT >= 64
	"u64",
	"s64",
	#endif
	#endif
	#endif
	"ul",
	"sl",
	"flt",
	"dou",
	"str",
	"obj"
};

/* Static Functions */
static inline char *sgets(
	char **out,
	size_t len,
	char *str
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

static inline enum clonc_types getType(
	const char *str,
	size_t len
) {
	unsigned char i;
	size_t typeLen;
	size_t strLen = strnlen(str, len);
	
	for (i = 0; i < CLONC_TYPE_COUNT; i++) {
		typeLen = strlen(clonc_typeNames[i]);
		
		if (
			strLen == typeLen &&
			strncmp(str, clonc_typeNames[i], strLen) == 0
		) {
			return (enum clonc_types)i;
		}
	}
	return CLONC_TYPE_UNSET;
}

/* Functions */
int clonc_init(
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

void clonc_uninit(clonc_var_t *var) {
	if (var == NULL) {
		fprintf(stderr, LANG_ERROR_REQUIRED_ARGS);
		return;
	}
	
	var->used = 0;
	var->type = CLONC_TYPE_UNSET;
	
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
void clonc_clearObj(clonc_obj_t *object) {
	VOARRAY_TYPE_SIZE i;
	
	if (object == NULL) {
		fprintf(stderr, LANG_ERROR_REQUIRED_ARGS);
		return;
	}
	
	for (i = 0; i < object->length; i++) {
		object->i[i].used = 0;
		object->i[i].type = CLONC_TYPE_VO;
		object->i[i].name = NULL;
		object->i[i].value.ptr = NULL;
	}
	return;
}

clonc_var_t *clonc_addObj(
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
		!(var->flags & VARFLAG_BM(VARFLAG_AD)) &&
		clonc_findObj(
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

clonc_var_t *clonc_findObj(
	clonc_obj_t *object,
	const char *name,
	VOARRAY_TYPE_SIZE *position
) {
	VOARRAY_TYPE_SIZE i = 0;
	
	if (
		object == NULL ||
		name == NULL
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

int clonc_uninitObj(clonc_obj_t *object) {
	VOARRAY_TYPE_SIZE i;
	
	for (i = 0; i < (object->length - 1); i++) {
		if (object->i[i].type == CLONC_TYPE_OBJ) {
			if (clonc_uninitObj(object->i[i].value.obj)) {
				return 1;
			}
		}
		clonc_uninit(&object->i[i]);
	}
	VOARRAY_UNINIT((*object));
	return 0;
}

/* From String */
int clonc_getStr(
	char *str,
	size_t len,
	clonc_var_t *var
) {
	char *prev = NULL;
	char *current = str;
	char *tmp[2];
	unsigned char i;
	
	for (i = 0; i < 2; i++) {
		tmp[i] = malloc(len);
	}
	
	do {
		prev = current;
		current = sgets(&current, len, str);
		
		// TODO: Replace `sscanf` with my own code that searches for the flag(s), type, name and value.
		sscanf(prev, "%s %s", tmp[0], tmp[1]);
		var->type = getType(tmp[0], len);
		
		if (var->type == CLONC_TYPE_UNSET) {
			return 1;
		}
	} while (current != NULL);
	
	for (i = 0; i < 2; i++) {
		free(tmp[i]);
	}
	return 0;
}