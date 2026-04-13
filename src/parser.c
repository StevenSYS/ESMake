#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "parser.h"

#define MALLOC_EC(_out, _len, _type, _ret) \
	_out = (_type)malloc(_len); \
	if (_out == NULL) { \
		fprintf(stderr, "ERROR: Failed to allocate memory\n"); \
		return _ret; \
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
	char noVal = 0;
	char *strCopy;
	char *prev = NULL;
	char *current;
	size_t tmpLen = 0;
	size_t varPos = 0;
	size_t i;
	section_t *ret = NULL;
	
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
		noVal = 0;
		prev = current;
		sgets(&current, len, strCopy);
		
		for (; prev[0] && isspace(prev[0]); prev++);
		
		switch (prev[0]) {
			case '+':
				if (foundSect) {
					goto end;
				}
				
				if (strncmp(prev + 1, name, len) == 0) {
					if (ret == NULL) {
						tmpLen = strlen(name);
						MALLOC_EC(
							ret,
							sizeof(section_t),
							section_t *,
							NULL
						);
						MALLOC_EC(
							ret->name,
							sizeof(char[tmpLen]),
							char *,
							NULL
						);
						strncpy(ret->name, name, tmpLen);
						ret->name[tmpLen] = 0;
						ret->vars.size = 0;
					}
					foundSect = 1;
				}
				continue;
			case '@':
				noVal = 1;
			case '%':
				if (foundSect) {
					if (counted) {
						for (i = 0; prev[i]; i++) {
							if (isspace(prev[i])) {
								prev[i] = 0;
								break;
							}
						}
						
						check = 0;
						
						if (!noVal) {
							for (i++; prev[i]; i++) {
								if (prev[i] == '\n') {
									break;
								} else if (prev[i] == '"') {
									check = 1;
									i++;
									break;
								}
							}
							
							if (!check) {
								fprintf(stderr, "ERROR: Missing start of value\n");
								return NULL;
							}
						}
						
						tmpLen = strlen(prev + 1),
						MALLOC_EC(
							ret->vars.i[varPos].name,
							sizeof(char[tmpLen]),
							char *,
							NULL
						);
						strncpy(ret->vars.i[varPos].name, prev + 1, tmpLen);
						ret->vars.i[varPos].name[tmpLen] = 0;
						
						if (noVal) {
							ret->vars.i[varPos].value = NULL;
						} else {
							prev = prev + i;
							
							check = 0;
							
							for (i = 0; prev[i]; i++) {
								if (prev[i] == '\n') {
									break;
								} else if (prev[i] == '"') {
									check = 1;
									prev[i] = 0;
									break;
								}
							}
							
							if (!check) {
								fprintf(stderr, "ERROR: Missing end of value\n");
								return NULL;
							}
							
							if (prev[0]) {
								tmpLen = strlen(prev),
								MALLOC_EC(
									ret->vars.i[varPos].value,
									sizeof(char[tmpLen]),
									char *,
									NULL
								);
								strncpy(ret->vars.i[varPos].value, prev, tmpLen);
								ret->vars.i[varPos].value[tmpLen] = 0;
							}
						}
						varPos++;
					} else {
						ret->vars.size++;
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
		ret->vars.size
	) {
		VOARRAY_INIT(var_t, ret->vars, NULL);
		counted = 1;
		goto setStr;
	}
	
	free(strCopy);
	
	if (ret == NULL) {
		fprintf(stderr, "ERROR: Failed to find section\n");
	}
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