#include <stdio.h>
#include <string.h>

#include "setting.h"
#include "whitespace.h"

int setting_add(
	settings_t *settings,
	const setting_t *setting
) {
	if (settings->count >= SETTING_LENGTH) {
		fprintf(stderr, "ERROR: No free slots in settings variable\n");
		return 1;
	}
	
	strncpy(
		settings->i[settings->count].name,
		setting->name,
		SETTING_LENGTH_STRING
	);
	strncpy(
		settings->i[settings->count].value,
		setting->value,
		SETTING_LENGTH_STRING
	);
	
	settings->count++;
	return 0;
}

int setting_getStr(
	settings_t *settings,
	char *str
) {
	char check = 0;
	char *value;
	unsigned int i;
	unsigned char j;
	
	setting_t tmpSet;
	
	for (i = 1; str[i]; i++) {
		if (whitespace(str[i])) {
			str[i] = 0;
			strncpy(
				tmpSet.name,
				str + 1,
				SETTING_LENGTH_STRING
			);
			break;
		}
	}
	
	for (j = 0; j < 2; j++) {
		check = 0;
		
		for (i++; str[i]; i++) {
			if (str[i] == '"') {
				if (j == 0) {
					value = str + (i + 1);
				} else {
					check = 1;
					str[i] = 0;
					strncpy(
						tmpSet.value,
						value,
						SETTING_LENGTH_STRING
					);
				}
				break;
			} else if (
				j == 0 &&
				!whitespace(str[i])
			) {
				fprintf(stderr, "ERROR: Non whitespace character before value string\n");
				return 1;
			}
		}
		
		if (
			j != 0 &&
			!check
		) {
			fprintf(stderr, "ERROR: Missing end of value string\n");
			return 1;
		}
	}
	setting_add(settings, &tmpSet);
	return 0;
}

setting_t *setting_find(
	settings_t *settings,
	const char *find
) {
	unsigned int i;
	
	for (i = 0; i < settings->count; i++) {
		if (strncmp(
			settings->i[i].name,
			find,
			SETTING_LENGTH_STRING
		) == 0) {
			return &settings->i[i];
		}
	}
	return NULL;
}