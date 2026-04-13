" Vim syntax file
" Language: ESMake
" Maintainer: StevenSYS
" Last Change: 13-04-2026

if exists("b:current_syntax")
	finish
endif

" Region
syn region esmString start="\"" end="[\"\n]"

" Match
syn match esmVar "[%@]\S\+"
syn match esmSection "[+-].\+"
syn region esmComment start="!" end="$"

hi def link esmString string

hi def link esmVar structure
hi def link esmSection keyword
hi def link esmComment comment

let b:current_syntax = "esmake"