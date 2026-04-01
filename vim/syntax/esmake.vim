" Vim syntax file
" Language: ESMake
" Maintainer: StevenSYS

if exists("b:current_syntax")
	finish
endif

" Region
syn region emString start="\"" end="\""

" Match
syn match emComment "//.\+"
syn match emNumber "\<\d\+\(_\d\+\)*\>"

" Keyword
syn keyword emVarType vo s8 u8 s16 u16 s32 u32 flt dou str obj

hi def link emString string

hi def link emComment comment
hi def link emNumber number

hi def link emVarType structure

let b:current_syntax = "esmake"