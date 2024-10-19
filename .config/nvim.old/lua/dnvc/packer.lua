vim.cmd [[packadd packer.nvim]]

return require("packer").startup(function(use)
  	
	-- Packer
	use 'wbthomason/packer.nvim'

    -- Telescope    
	use {
		"nvim-telescope/telescope.nvim", tag = "0.1.1",
		requires = { {"nvim-lua/plenary.nvim"} }
	}

	-- Themes
	use ({
			"rafi/awesome-vim-colorschemes",
			config = function ()
				vim.cmd("colorscheme onedark") -- Set your colour schemes here!
			end
	})
	
	-- Treesitter
	use ({
		"nvim-treesitter/nvim-treesitter", 
		{ run = "TSUpdate" }
	})

	use "mbbill/undotree"
	use "tpope/vim-fugitive"

	-- Lanague Server
	use {
  			'VonHeikemen/lsp-zero.nvim',
  			branch = 'v1.x',
  			requires = {
    			-- LSP Support
    			{'neovim/nvim-lspconfig'},             -- Required
    			{'williamboman/mason.nvim'},           -- Optional
    			{'williamboman/mason-lspconfig.nvim'}, -- Optional
	
    			-- Autocompletion
    			{'hrsh7th/nvim-cmp'},         -- Required
    			{'hrsh7th/cmp-nvim-lsp'},     -- Required
    			{'hrsh7th/cmp-buffer'},       -- Optional
    			{'hrsh7th/cmp-path'},         -- Optional
    			{'saadparwaiz1/cmp_luasnip'}, -- Optional
    			{'hrsh7th/cmp-nvim-lua'},     -- Optional

    			-- Snippets
    			{'L3MON4D3/LuaSnip'},             -- Required
    			{'rafamadriz/friendly-snippets'}, -- Optional
  		}
	}

    -- Trouble
	use {
		"folke/trouble.nvim",
		requires = "nvim-tree/nvim-web-devicons"
	}

	-- Cool
	use "vim-airline/vim-airline"
	use "preservim/nerdtree"
	use "ryanoasis/vim-devicons"
	use "tc50cal/vim-terminal"
	use "preservim/tagbar"
	use "mg979/vim-visual-multi"
	use "romgrk/barbar.nvim"
	use "nvim-tree/nvim-web-devicons"

end)
