#ifndef __VARIABLE__
#define __VARIABLE__

#include <stdint.h>

#include "VoArray/voArray.h"

/* Enums */
enum variable_types {
	VARTYPE_VO,
	VARTYPE_S8,
	VARTYPE_U8,
	VARTYPE_S16,
	VARTYPE_U16,
	VARTYPE_S32,
	VARTYPE_U32,
	VARTYPE_FLT,
	VARTYPE_DOU,
	VARTYPE_STR,
	VARTYPE_OBJ,
	
	VARTYPE_COUNT
};

/* VoArrays */
VOARRAY(varObj_t, struct variable_s);

/* Structs */
typedef struct variable_s {
	char used;
	enum variable_types type;
	char *name;
	union {
		int8_t *s8;
		uint8_t *u8;
		int16_t *s16;
		uint16_t *u16;
		int32_t *s32;
		uint32_t *u32;
		float *flt;
		double *dou;
		char *str;
		varObj_t *obj;
		
		void *ptr;
	} value;
} variable_t;

/* Functions */
int variable_init(
	variable_t *var,
	size_t nameSize,
	size_t valSize
);

void variable_uninit(variable_t *var);

/* Object */
void variable_clearObj(varObj_t *object);

variable_t *variable_addObj(
	varObj_t *object,
	variable_t *var,
	VOARRAY_TYPE_SIZE *i
);

variable_t *variable_findObj(
	varObj_t *object,
	const char *name,
	VOARRAY_TYPE_SIZE *i
);

int variable_uninitObj(varObj_t *object);

/* From File */
int variable_getFile(
	FILE *file,
	variable_t *var
);

#ifndef VARIABLE_NOEXTERNS
/* Externs */
extern const char *variable_typeNames[VARTYPE_COUNT];
#endif

#endif