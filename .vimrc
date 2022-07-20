set textwidth=76

"always show the sign column
set scl=yes
"https://www.ditig.com/256-colors-cheat-sheet
"https://vim.fandom.com/wiki/Xterm256_color_names_for_console_Vim
:highlight SignColumn ctermbg=236

"see :h ycm_add_preview_to_completeopt
set completeopt+=popup

let g:ycm_confirm_extra_conf = 0

"some YouCompleteMe key bindings
nnoremap gd :YcmCompleter GoToDefinition<cr>

nnoremap <F3> gg=G''<cr>
nnoremap <F4> :make<cr>
nnoremap <F5> :make! run<cr>
