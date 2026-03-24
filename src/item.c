#include <string.h>

#include "item.h"
#include "progInfo.h"
#include "whitespace.h"


int item_addFile(
	FILE *files[],
	unsigned int fileCount,
	unsigned int itemCount,
	char *str,
	enum modes mode,
	settings_t *settings
) {
	char check = 0;
	char *value;
	unsigned int i;
	unsigned char j;
	static unsigned int currentCount = 0;
	static setting_t *preEnum = NULL;
	static setting_t *preLang = NULL;
	
	if (
		files == NULL ||
		str == NULL ||
		settings == NULL
	) {
		fprintf(stderr, "ERROR: Required argument(s) is/are NULL\n");
		return 1;
	}
	
	if (
		mode != MODE_LANG &&
		preEnum == NULL
	) {
		preEnum = setting_find(settings, SETTING_PREENUM);
		
		if (preEnum == NULL) {
			fprintf(stderr, "ERROR: \"" SETTING_PREENUM "\" isn't set\n");
			return 1;
		}
	}
	
	if (preLang == NULL) {
		preLang = setting_find(settings, SETTING_PRELANG);
		
		if (preLang == NULL) {
			fprintf(stderr, "ERROR: \"" SETTING_PRELANG "\" isn't set\n");
			return 1;
		}
	}
	
	switch (mode) {
		case MODE_ENUM:
			/* Ret */
			for (i = 0; str[i]; i++) {
				if (whitespace(str[i])) {
					str[i] = 0;
					break;
				}
			}
			
			if (str[0] == LEGEND_ITEMDEFAULT) {
				fprintf(
					files[0],
					"default: return %s%s;\n",
					preLang->value,
					str + 1
				);
			} else {
				fprintf(
					files[0],
					"case %s%s: return %s%s;\n",
					preEnum->value,
					str + 1,
					preLang->value,
					str + 1
				);
			}
			
			/* Enum */
			fprintf(
				files[1],
				"%s%s",
				preEnum->value,
				str + 1
			);
			
			if (currentCount < (itemCount - 1)) {
				fprintf(files[1], ",");
			}
			
			fprintf(files[1], "\n");
			break;
		case MODE_LANG:
			for (i = 0; str[i]; i++) {
				if (whitespace(str[i])) {
					str[i] = 0;
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
			
			fprintf(
				files[0],
				"#define %s%s \"%s\"\n",
				preLang->value,
				str + 1,
				value
			);
			break;
		default:
			fprintf(stderr, "ERROR: Unknown mode: %d\n", mode);
			return 1;
	}
	currentCount++;
	return 0;
}