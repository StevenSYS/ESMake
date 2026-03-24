#ifndef __ITEM__
#define __ITEM__

#include <stdio.h>

#include "mode.h"
#include "setting.h"

/* Functions */
int item_addFile(
	FILE *files[],
	unsigned int fileCount,
	unsigned int itemCount,
	char *str,
	enum modes mode,
	settings_t *settings
);

#endif