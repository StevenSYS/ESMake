#ifndef __MODE__
#define __MODE__

/* Enums */
enum modes {
	MODE_ENUM,
	MODE_LANG,
	
	MODE_COUNT
};

#ifndef MODE_NOEXTERNS
/* Externs */
extern const char *mode_names[MODE_COUNT];
#endif

/* Functions */
int mode_getStr(
	const char *str,
	enum modes *mode
);

#endif