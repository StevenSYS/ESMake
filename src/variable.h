#ifndef __VARIABLE__
#define __VARIABLE__

/* Macros */
#define LENGTH_VARTYPE 10
#define LENGTH_BUFFER 255

/* Enums */
enum variable_types {
	VARTYPE_VO,
	VARTYPE_S8,
	VARTYPE_U8,
	VARTYPE_S16,
	VARTYPE_U16,
	VARTYPE_S32,
	VARTYPE_U32,
	VARTYPE_STR,
	VARTYPE_OBJ,
	VARTYPE_ARR
};

/* Structs */
typedef struct {
	enum variable_types type;
	char name[LENGTH_BUFFER];
	void *value;
} variable_t;

#endif