#ifndef __CLONC_CLON__
#define __CLONC_CLON__

#include <stdint.h>

#include "VoArray/voArray.h"

/* Macros */
#ifndef CLONC_BIT
	#ifdef INTPTR_MAX
		#if INTPTR_MAX >= INT64_MAX
			#define CLONC_BIT 64
		#elif INTPTR_MAX >= INT32_MAX
			#define CLONC_BIT 32
		#elif INTPTR_MAX >= INT16_MAX
			#define CLONC_BIT 16
		#endif
	#else
		#warn "Missing INTPTR_MAX macro, guessing 32-bit"
		#define CLONC_BIT 32
	#endif
#endif

/* Enums */
enum clonc_types {
	CLONC_TYPE_VO,  // void
	CLONC_TYPE_U8,  // uint8_t
	CLONC_TYPE_S8,  // sint8_t
	#if CLONC_BIT >= 16
	CLONC_TYPE_U16, // uint16_t
	CLONC_TYPE_S16, // sint16_t
	#if CLONC_BIT >= 32
	CLONC_TYPE_U32, // uint32_t
	CLONC_TYPE_S32, // sint32_t
	#if CLONC_BIT >= 64
	CLONC_TYPE_U64, // uint64_t
	CLONC_TYPE_S64, // sint64_t
	#endif
	#endif
	#endif
	CLONC_TYPE_UL,  // size_t
	CLONC_TYPE_SL,  // ssize_t
	CLONC_TYPE_FLT, // float
	CLONC_TYPE_DOU, // double
	CLONC_TYPE_STR, // char *
	CLONC_TYPE_OBJ, // clonc_obj_t
	
	CLONC_TYPE_COUNT,
	
	/* Specials */
	CLONC_TYPE_UNSET
};

/* VoArrays */
VOARRAY(clonc_obj_t, struct clonc_var_s);

/* Structs */
typedef struct clonc_var_s {
	char used;
	unsigned char flags;
	enum clonc_types type;
	char *name;
	union {
		size_t *ul;
		ssize_t *sl;
		float *flt;
		double *dou;
		char *str;
		clonc_obj_t *obj;
		
		void *ptr;
	} value;
} clonc_var_t;

/* Functions */
int clonc_init(
	clonc_var_t *var,
	size_t nameSize,
	size_t valSize
);

void clonc_uninit(clonc_var_t *var);

/* Object */
void clonc_clearObj(clonc_obj_t *object);

clonc_var_t *clonc_addObj(
	clonc_obj_t *object,
	clonc_var_t *var,
	VOARRAY_TYPE_SIZE *position
);

clonc_var_t *clonc_findObj(
	clonc_obj_t *object,
	const char *name,
	VOARRAY_TYPE_SIZE *position
);

int clonc_uninitObj(clonc_obj_t *object);

/* From String */
int clonc_getStr(
	char *file,
	size_t len,
	clonc_var_t *var
);

#ifndef CLONC_NOEXTERNS
/* Externs */
extern const char *clonc_typeNames[CLONC_TYPE_COUNT];
#endif

#endif