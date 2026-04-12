#define VARFLAG_NOEXTERNS
#include <stdio.h>
#include <string.h>

#include "lang.h"
#include "varFlag.h"

const char *varFlag_names[VARFLAG_COUNT] = {
	"nn", // No Name
	"ad"  // Allow Duplicate
};

enum varFlags varFlag_check(
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
	
	for (i = 0; i < VARFLAG_COUNT; i++) {
		len = strnlen(str, maxLen);
		nameLen = strlen(varFlag_names[i]);
		
		if (
			len == nameLen &&
			strncmp(str, varFlag_names[i], len) == 0
		) {
			return (enum varFlags)i;
		}
	}
	return VARFLAG_NONE;
}