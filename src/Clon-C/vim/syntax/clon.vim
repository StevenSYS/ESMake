" Vim syntax file
" Language: Clon
" Maintainer: StevenSYS

if exists("b:current_syntax")
	finish
endif

" Region
syn region clonString start="\"" end="\""
syn region clonComment start="//" end="$"

" Match
syn match clonNumber "\<\d\+\(_\d\+\)*\>"

" Keyword
syn keyword clonVarType vo in flt dou str obj
syn keyword clonVarFlag nn ad

hi def link clonString string
hi def link clonComment comment

hi def link clonNumber number

hi def link clonVarType structure
hi def link clonVarFlag keyword

let b:current_syntax = "clon"