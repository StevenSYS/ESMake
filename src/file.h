#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>

#include "VoArray/voArray.h"

/* Functions */
int file_open(
	FILE **output,
	const char *filename,
	const char *mode
);

/* VoArrays */
VOARRAY(files_t, FILE *);

#endif