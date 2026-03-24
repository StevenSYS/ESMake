" Vim syntax file
" Language: ESMake
" Maintainer: StevenSYS
" Last Change: 23-03-2026

if exists("b:current_syntax")
	finish
endif

runtime! syntax/c.vim

syn match emOutput "^>"
syn match emSetting "^%\S\+"
syn match emItem "^[+|-]\S\+"
syn match emEscape "^\\.\+"
syn region emComment start="^!" skip="\\$" end="$"

" Settings
hi def link emOutput keyword
hi def link emSetting keyword
hi def link emItem structure
hi def link emEscape special
hi def link emComment comment

let b:current_syntax = "esmake"