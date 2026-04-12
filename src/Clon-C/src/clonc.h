#ifndef __CLONC__
#define __CLONC__

#include "VoArray/voArray.h"

/* VoArrays */
VOARRAY(clonc_obj_t, struct clonc_var_s);

/* Structs */
typedef struct clonc_var_s {
	char used;
	unsigned char flags;
	unsigned char type;
	char *name;
	union {
		int *in;
		float *flt;
		double *dou;
		char *str;
		clonc_obj_t *obj;
		
		void *ptr;
	} value;
} clonc_var_t;

/* Functions */

/* Variable */
int clonc_varInit(
	clonc_var_t *var,
	size_t nameSize,
	size_t valSize
);

int clonc_varUninit(clonc_var_t *var);

/* Object */
void clonc_objClear(clonc_obj_t *obj);
int clonc_objUninit(clonc_obj_t *obj);

/* Object - Variable */
clonc_var_t *clonc_objAddVar(
	clonc_obj_t *obj,
	clonc_var_t *var,
	VOARRAY_TYPE_SIZE *position
);

clonc_var_t *clonc_objFindVar(
	clonc_obj_t *obj,
	const char *name,
	VOARRAY_TYPE_SIZE *position
);

/* Object - String */
int clonc_objFromStr(
	const char *str,
	size_t len,
	clonc_obj_t *obj
);

#endif