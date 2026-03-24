#ifndef __SETTING__
#define __SETTING__

/* Macros */
#define SETTING_LENGTH 3
#define SETTING_LENGTH_STRING 100

/* Structs */
typedef struct {
	char name[SETTING_LENGTH_STRING];
	char value[SETTING_LENGTH_STRING];
} setting_t;

typedef struct {
	unsigned int count;
	setting_t i[SETTING_LENGTH];
} settings_t;

/* Functions */
int setting_add(
	settings_t *settings,
	const setting_t *setting
);

int setting_getStr(
	settings_t *settings,
	char *str
);

setting_t *setting_find(
	settings_t *settings,
	const char *find
);

#endif