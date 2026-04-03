" Vim syntax file
" Language: ESMake
" Maintainer: StevenSYS

if exists("b:current_syntax")
	finish
endif

" Region
syn region emString start="\"" end="\""
syn region emStop start="!" end=".+"

" Match
syn match emNumber "\<\d\+\(_\d\+\)*\>"

" Keyword
syn keyword emVarType vo num flt dou str obj
syn keyword emVarFlag nn ad

hi def link emString string
hi def link emStop comment

hi def link emNumber number

hi def link emVarType structure
hi def link emVarFlag keyword

let b:current_syntax = "esmake"