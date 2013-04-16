set makeprg+=\ -C$PWD/build
set noet
set sw=4
set ts=8
set tw=80

au BufRead,BufNewFile *.md set filetype=markdown

let g:alternateSearchPath='sfr:.,reg:/include/src/,reg:/src/include/'
