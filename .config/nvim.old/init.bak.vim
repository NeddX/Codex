" Properties and stuff...
set paste
set number
set ts=4
set sw=4
set title
set mouse=a
set softtabstop=4
set smarttab
set shiftwidth=4
set encoding=UTF8
set completeopt=menuone,longest
set cursorline
set wildmenu
set clipboard=unnamedplus
syntax on
set t_Co=256
filetype plugin indent on

" Fix the copying and pasting bullshit arsehole developer's of vim don't fix
vnoremap <C-S-c> "+y

" Path to init.vim
"let appdata = fnamemodify($MYVIMRC, ":h");

" Plugin stuff
call plug#begin("~/.config/nvim/plugged/")

Plug 'https://github.com/vim-airline/vim-airline'
Plug 'http://github.com/tpope/vim-surround' " Surrounding ysw)
Plug 'https://github.com/preservim/nerdtree' " NerdTree
Plug 'https://github.com/tpope/vim-commentary' " For Commenting gcc & gc
"Plug 'https://github.com/lifepillar/pgsql.vim' " PSQL Pluging needs :SQLSetType pgsql.vim
Plug 'https://github.com/ap/vim-css-color' " CSS Color Preview
Plug 'https://github.com/rafi/awesome-vim-colorschemes' " Retro Scheme
"Plug 'https://github.com/neoclide/coc.nvim'  " Auto Completion
Plug 'https://github.com/ryanoasis/vim-devicons' " Developer Icons
Plug 'https://github.com/tc50cal/vim-terminal' " Vim Terminal
Plug 'https://github.com/preservim/tagbar' " Tagbar for code navigation
Plug 'https://github.com/mg979/vim-visual-multi' " New multi cursor
Plug 'https://github.com/romgrk/barbar.nvim' " Tab plugin
Plug 'nvim-tree/nvim-web-devicons'

inoremap <silent><expr> <c-space> coc#refresh()

" Configure compe from a luafile
"luafile compe_config.lua

" Fix broken icons
let g:NERDTreeDirArrowExpandable=""
let g:NERDTreeDirArrowCollapsible=""

" Keymap NERDTree
nnoremap <C-f> :NERDTreeFocus<CR>
nnoremap <C-n> :NERDTree<CR>
nnoremap <C-t> :NERDTreeToggle<CR>
nnoremap <C-l> :call CocActionAsync('jumpDefinition')<CR>

" Keymap Tagbar
nmap <F8> :TagbarToggle<CR>

call plug#end()

" Set the theme
colorscheme onedark
" Good themes: onedark edge jellybeans onehalf

" =======================================
" Stuff I dont want to touch.
" Reset the cursor to block after leaving
autocmd BufWinLeave * set guicursor=i:blinkon5

" No idea what this is
let &t_SI = "\<Esc>]50;CursorShape=1\x7"
let &t_SR = "\<Esc>]50;CursorShape=2\x7"
let &t_EI = "\<Esc>]50;CursorShape=0\x7"

" Set Cursor to blink in INSERT mode
set guicursor=i:blinkon5
" Set Cursor to be UNDERLINE in INSERT mode
set guicursor+=n:ver20-Cursor/lCursor

" Line highlighting stuff...
highlight LineNr term=bold cterm=NONE ctermfg=DarkGray ctermbg=NONE gui=NONE
highlight CursorLineNr term=bold cterm=NONE ctermfg=White ctermbg=DarkGray cterm=NONE 
highlight CursorLine term=bold cterm=bold ctermbg=DarkGray
highlight TermCursor guifg=White ctermfg=White

" Italics
let &t_ZH="\e[3m"
let &t_ZR="\e[23g"

" Function to tell you the current scheme
function! ShowColourSchemeName()
    try
        echo g:colors_name
    catch /^Vim:E121/
        echo "default"
    endtry
endfunction

