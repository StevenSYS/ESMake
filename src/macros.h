#ifndef __MACROS__
#define __MACROS__
	#define MALLOC_EC(_out, _len, _type, _ret) \
		_out = (_type)malloc(_len); \
		if (_out == NULL) { \
			fprintf(stderr, "ERROR: Failed to allocate memory\n"); \
			return _ret; \
		}
	
	#define STRCLONE(_out, _str) \
		tmpLen = strlen(_str); \
		MALLOC_EC( \
			_out, \
			sizeof(char[tmpLen]), \
			char *, \
			NULL \
		); \
		strncpy(_out, _str, tmpLen); \
		_out[tmpLen] = 0;
#endif