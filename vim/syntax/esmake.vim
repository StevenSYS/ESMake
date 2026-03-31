" Vim syntax file
" Language: ESMake
" Maintainer: StevenSYS
" Last Change: 23-03-2026

if exists("b:current_syntax")
	finish
endif

runtime! syntax/c.vim

syn match emVariable "^[%>]\S\+"
syn match emItem "^[+-]\S\+"
syn match emItemOut "^|\S\+"
syn match emEscape "^\\.\+"
syn region emComment start="^!" skip="\\$" end="$"

" Settings
hi def link emVariable keyword
hi def link emItem structure
hi def link emItemOut function
hi def link emEscape special
hi def link emComment comment

let b:current_syntax = "esmake"