vim.api.nvim_set_keymap('n', '<C-f>', ':NERDTreeFocus<CR>', {noremap = true})
vim.api.nvim_set_keymap('n', '<C-n>', ':NERDTree<CR>', {noremap = true})
vim.api.nvim_set_keymap('n', '<C-t>', ':NERDTreeToggle<CR>', {noremap = true})
vim.api.nvim_set_keymap('n', '<C-l>', ':call CocActionAsync("jumpDefinition")<CR>', {noremap = true})
