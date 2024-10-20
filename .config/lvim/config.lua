-- Read the docs: https://www.lunarvim.org/docs/configuration
-- Example configs: https://github.com/LunarVim/starter.lvim
-- Video Tutorials: https://www.youtube.com/watch?v=sFA9kX-Ud_c&list=PLhoH5vyxr6QqGu0i7tt_XoVK9v-KvZ3m6
-- Forum: https://www.reddit.com/r/lunarvim/
-- Discord: https://discord.com/invite/Xb9B4Ny

lvim.plugins = {
    -- Colourschemes
    { "rafamadriz/neon" },
    { "rebelot/kanagawa.nvim" },
    { "bluz71/vim-moonfly-colors" },
    { "navarasu/onedark.nvim" },
    { "marko-cerovac/material.nvim" },
    { "tiagovla/tokyodark.nvim" },
    { "miikanissi/modus-themes.nvim" },
    { "rcarriga/nvim-dap-ui" },
    { "theHamsta/nvim-dap-virtual-text" },
    { "nvim-lua/plenary.nvim" },
    { "stevearc/overseer.nvim" },
    { "Shatur/neovim-ayu" },
    {
        "ray-x/lsp_signature.nvim",
        event = "InsertEnter",
        opts = {
            bind = true,
            handler_opt = {
                border = "rounded"
            }
        },
        config = function(_, opts) require 'lsp_signature'.setup(opts) end
    },
    {
        "neovim/nvim-lspconfig"
    }
    --{ "Civitasv/cmake-tools.nvim" }
}

lvim.transparent_window = false
--lvim.colorscheme = "moonfly";
lvim.colorscheme = "ayu-dark";
lvim.builtin.lualine.style = "lvim"

vim.lsp.inlay_hint.enable(true)
vim.opt.autoindent = true
vim.opt.smartindent = true
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true
vim.opt.cursorline = true

-- Disable line numbers in terminal buffers
vim.api.nvim_create_autocmd("TermOpen", {
    pattern = "*",
    callback = function()
        vim.wo.number = false         -- Disable line numbers
        vim.wo.relativenumber = false -- Disable relative line numbers (if enabled)
    end,
})

lvim.builtin.treesitter.ident = { enable = true }
lvim.format_on_save.enabled = true
