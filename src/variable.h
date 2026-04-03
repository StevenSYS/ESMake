#ifndef __VARIABLE__
#define __VARIABLE__

#include <stdint.h>

#include "VoArray/voArray.h"

/* Macros */

/* - Char - */
#define VARIABLE_CHAR_END ';'

/* Enums */
enum variable_types {
	VARTYPE_VO,  // void
	VARTYPE_S8,  // int8_t
	VARTYPE_U8,  // uint8_t
	VARTYPE_S16, // int16_t
	VARTYPE_U16, // uint16_t
	VARTYPE_S32, // int32_t
	VARTYPE_U32, // uint32_t
	VARTYPE_FLT, // float
	VARTYPE_DOU, // double
	VARTYPE_STR, // char *
	VARTYPE_OBJ, // varObj_t
	
	VARTYPE_COUNT,
	
	/* Specials */
	VARTYPE_UNSET
};

/* VoArrays */
VOARRAY(varObj_t, struct variable_s);

/* Structs */
typedef struct variable_s {
	char used;
	unsigned char flags;
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
	VOARRAY_TYPE_SIZE *position
);

variable_t *variable_findObj(
	varObj_t *object,
	const char *name,
	VOARRAY_TYPE_SIZE *position
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