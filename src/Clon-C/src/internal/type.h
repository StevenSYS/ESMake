#ifndef __TYPE__
#define __TYPE__

#include <stdint.h>

/* Enums */
enum clonc_types {
	TYPE_VO,  // void
	TYPE_IN,  // int
	TYPE_FLT, // float
	TYPE_DOU, // double
	TYPE_STR, // char *
	TYPE_OBJ, // clonc_obj_t
	
	TYPE_COUNT,
	
	/* Specials */
	TYPE_UNKNOWN
};

/* Functions */
enum clonc_types type_get(
	const char *str,
	size_t maxLen
);

#ifndef TYPE_NOEXTERNS
/* Externs */
extern const char *type_names[TYPE_COUNT];
#endif

#endif