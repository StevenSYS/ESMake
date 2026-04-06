" Vim syntax file
" Language: Clon
" Maintainer: StevenSYS

if exists("b:current_syntax")
	finish
endif

" Region
syn region clonString start="\"" end="\""
syn region clonStop start="//" end="$"

" Match
syn match clonNumber "\<\d\+\(_\d\+\)*\>"

" Keyword
syn keyword clonVarType vo u8 s8 u16 s16 u32 s32 u64 s64 ul sl flt dou str obj
syn keyword clonVarFlag nn ad

hi def link clonString string
hi def link clonStop comment

hi def link clonNumber number

hi def link clonVarType structure
hi def link clonVarFlag keyword

let b:current_syntax = "clon"