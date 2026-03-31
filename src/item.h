#ifndef __ITEM__
#define __ITEM__

#include <stdio.h>

#include "mode.h"
#include "file.h"
#include "variable.h"

/* Functions */
int item_addFile(
	const char *buffer,
	files_t *files,
	variables_t *outVars,
	variables_t *vars
);

#endif