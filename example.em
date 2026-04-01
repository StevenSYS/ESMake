>ret "exampleRet.h"
>enum "exampleEnum.h"
>lang "exampleLang.h"
>lang2 "exampleLang2.h"
%preEnum "VISION_ERROR_"
%preLang "LANG_ERROR_"
! | Legend | Meaning                     |
! | ------ | --------------------------- |
! | >      | Output Variable             |
! | %      | Variable                    |
! | +      | Item                        |
! | -      | Item (Switch Default)       |
! | |      | Item - Lang Output          |
! | !      | Comment                     |
! | \      | Escape                      |
+EXAMPLE
|lang "lang - Example error"
|lang2 "lang2 - Example error"

/* This item is the default in the return switch */
-DEFEXAMPLE
|lang "lang - Default error"
|lang2 "lang2 - Default error"