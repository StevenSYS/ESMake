#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "parser.h"
#include "macros.h"
#include "progInfo.h"

#define FINDQUOTE() \
	for (; prev[i]; i++) { \
		if (prev[i] == '\n') { \
			break; \
		} else if (prev[i] == '"') { \
			check = 1; \
			break; \
		} \
	}

static inline char *sgets(
	char **out,
	size_t len,
	char *str
) {
	if (
		out == NULL ||
		str == NULL ||
		!len
	) {
		return NULL;
	}
	
	*out = (char *)memchr(
		(const char *)str,
		(int)'\n',
		len
	);
	
	if (*out != NULL) {
		*out[0] = 0;
		(*out)++;
	}
	return *out;
}

section_t *parser_getSection(
	const char *str,
	size_t len,
	const char *name
) {
	char check = 0;
	char counted = 0;
	char foundSect = 0;
	char *strCopy;
	char *prev = NULL;
	char *current;
	size_t tmpLen = 0;
	size_t varPos = 0;
	size_t i;
	section_t *ret = NULL;
	
	#define SIZE ret->vars.size
	#define NAME ret->vars.i[varPos].name
	#define VALUE ret->vars.i[varPos].value
	
	if (
		str == NULL ||
		!len ||
		name == NULL
	) {
		return NULL;
	}
	
	MALLOC_EC(
		strCopy,
		sizeof(char[len]),
		char *,
		NULL
	);
	
	setStr:
	foundSect = 0;
	
	strncpy(strCopy, str, len);
	current = strCopy;
	
	while (current != NULL) {
		prev = current;
		sgets(&current, len, strCopy);
		
		for (; prev[0] && isspace(prev[0]); prev++);
		
		switch (prev[0]) {
			case LEGEND_SECTION:
				if (foundSect) {
					goto end;
				}
				
				if (strncmp(prev + 1, name, len) == 0) {
					if (ret == NULL) {
						MALLOC_EC(
							ret,
							sizeof(section_t),
							section_t *,
							NULL
						);
						STRCLONE(ret->name, name);
						SIZE = 0;
					}
					foundSect = 1;
				}
				continue;
			case LEGEND_VAR_NOVAL:
			case LEGEND_VAR:
				if (foundSect) {
					if (counted) {
						for (i = 0; prev[i]; i++) {
							if (isspace(prev[i])) {
								prev[i] = 0;
								break;
							}
						}
						
						check = 0;
						
						if (prev[0] != LEGEND_VAR_NOVAL) {
							i++;
							FINDQUOTE();
							i++;
							
							if (!check) {
								fprintf(stderr, "ERROR: Missing start of value: %s\n", name);
								return NULL;
							}
						}
						
						STRCLONE(NAME, prev + 1);
						
						if (prev[0] == LEGEND_VAR_NOVAL) {
							VALUE = NULL;
						} else {
							prev = prev + i;
							
							check = 0;
							
							i = 0;
							FINDQUOTE();
							prev[i] = 0;
							
							if (!check) {
								fprintf(stderr, "ERROR: Missing end of value: %s\n", NAME);
								return NULL;
							}
							
							if (prev[0]) {
								STRCLONE(VALUE, prev);
							}
						}
						varPos++;
					} else {
						SIZE++;
					}
				}
				continue;
			default:
				continue;
		}
	}
	end:
	if (
		!counted &&
		ret != NULL &&
		SIZE
	) {
		VOARRAY_INIT(var_t, ret->vars, NULL);
		counted = 1;
		goto setStr;
	}
	
	free(strCopy);
	
	if (ret == NULL) {
		fprintf(stderr, "ERROR: Failed to find section: %s\n", name);
	}
	
	#undef SIZE
	#undef NAME
	#undef VALUE
	return ret;
}

size_t parser_getVar(
	const section_t *sect,
	const char *name,
	size_t len,
	size_t offset,
	char *check
) {
	size_t i;
	size_t varLen;
	
	*check = 1;
	
	for (i = offset; i < sect->vars.size; i++) {
		if (sect->vars.i[i].name == NULL) {
			continue;
		}
		
		varLen = strlen(sect->vars.i[i].name);
		
		if (
			len == varLen &&
			strncmp(
				sect->vars.i[i].name,
				name,
				len
			) == 0
		) {
			*check = 0;
			return i;
		}
	}
	return 0;
}

void parser_uninitVar(var_t *var) {
	if (var->name != NULL) {
		free(var->name);
		var->name = NULL;
	}
	
	if (var->value != NULL) {
		free(var->value);
		var->value = NULL;
	}
	return;
}

void parser_uninitSection(section_t *sect) {
	size_t i;
	
	for (i = 0; i < sect->vars.size; i++) {
		parser_uninitVar(&sect->vars.i[i]);
	}
	VOARRAY_UNINIT(sect->vars);
	
	free(sect->name);
	free(sect);
	return;
}