#define MODE_NOEXTERNS
#include <stdio.h>
#include <string.h>

#include "mode.h"

const char *mode_names[MODE_COUNT] = {
	"enum",
	"lang"
};

int mode_getStr(
	const char *str,
	enum modes *mode
) {
	char check = 0;
	unsigned int i;
	
	for (i = 0; i < MODE_COUNT; i++) {
		if (strcmp(str, mode_names[i]) == 0) {
			check = 1;
			*mode = (enum modes)i;
			break;
		}
	}
	
	if (!check) {
		fprintf(stderr, "ERROR: Unknown mode: %s\n", str);
		return 1;
	}
	return 0;
}