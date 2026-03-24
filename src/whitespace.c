char whitespace(char chr) {
	switch (chr) {
		case ' ':
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r':
			return 1;
		default:
			return 0;
	}
	return 0;
}