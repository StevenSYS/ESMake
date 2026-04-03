#ifndef __VARFLAG__
#define __VARFLAG__

/* MACROS */
#define VARFLAG_BM(_val) (1 << _val)

/* Enums */
enum varFlags {
	VARFLAG_NN,
	VARFLAG_AD,
	
	VARFLAG_COUNT,
	
	/* Specials */
	VARFLAG_NONE
};

/* Functions */
enum varFlags varFlag_check(
	const char *str,
	size_t maxLen
);

#ifndef VARFLAG_NOEXTERNS
/* Externs */
extern const char *varFlag_names[VARFLAG_COUNT];
#endif

#endif