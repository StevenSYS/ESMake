#ifndef __ITEM__
#define __ITEM__

#include <stdio.h>

#include "mode.h"
#include "file.h"
#include "variable.h"

/* Functions */
void item_preInit();
int item_init(
	files_t *files,
	variables_t *outVars,
	variables_t *vars
);

int item_cleanName(char *str);
int item_add(
	const char *name,
	char def,
	size_t count
);

#endif