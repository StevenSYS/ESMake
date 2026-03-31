#ifndef __VERBOSE__
#define __VERBOSE__

#include <stdio.h>

/* Macros */
#ifndef __FILE_NAME__
	#define __FILE_NAME__ __FILE__
#endif

#define VERBOSE_PRINTF(...) \
	if (verbose_enabled) { \
		printf( \
			"%s: %i - %s: ", \
			__FILE_NAME__, \
			__LINE__, \
			__func__ \
		); \
		printf(__VA_ARGS__); \
	}

/* Functions */
extern char verbose_enabled;

#endif