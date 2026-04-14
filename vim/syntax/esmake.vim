" Vim syntax file
" Language: ESMake
" Maintainer: StevenSYS
" Last Change: 14-04-2026

if exists("b:current_syntax")
	finish
endif

" Region
syn region esmValue start="\"" end="[\"\n]"
syn region esmComment start="^\s*!" end="$"

" Match
syn match esmVar "^\s*[%@]\S\+"
syn match esmSection "^\s*+.\+"

hi def link esmValue string
hi def link esmComment comment

hi def link esmVar structure
hi def link esmSection keyword

let b:current_syntax = "esmake"