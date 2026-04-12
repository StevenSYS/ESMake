#define CLONC_TYPE_NOEXTERNS
#include <stdio.h>
#include <string.h>

#include "lang.h"
#include "type.h"

const char *type_names[TYPE_COUNT] = {
	"vo",
	"in",
	"flt",
	"dou",
	"str",
	"obj"
};

enum clonc_types type_get(
	const char *str,
	size_t maxLen
) {
	size_t i;
	size_t len = 0;
	size_t nameLen = 0;
	
	if (str == NULL) {
		fprintf(stderr, LANG_ERROR_REQUIRED_ARGS);
		return 1;
	}
	
	for (i = 0; i < TYPE_COUNT; i++) {
		len = strnlen(str, maxLen);
		nameLen = strlen(type_names[i]);
		
		if (
			len == nameLen &&
			strncmp(str, type_names[i], len) == 0
		) {
			return (enum clonc_types)i;
		}
	}
	return TYPE_UNKNOWN;
}