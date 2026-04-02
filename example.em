obj outputs {
	str ret "ret.h";
	str enum "enum.h";
	str lang1 "lang1.h";
	str lang2 "lang2.h";
};

obj variables {
	str preEnum "ERROR_";
	str preLang "LANG_ERROR_";
};

obj items {
	obj EXAMPLE {
		str lang1 "lang1 - Example";
		str lang2 "lang2 - Example";
	};
	
	// This is the default in the return switch
	obj DEFEXAMPLE {
		vo def;
		str lang1 "lang1 - Default Example";
		str lang2 "lang2 - Default Example";
	};
};