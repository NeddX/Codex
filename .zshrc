# My stuff

#cowsay -f tux "The talking penguin." | lolcat

export CC=clang
export CXX=clang++

export PLATFORM_LOGO="?"
export PLATFORM_OS="$(uname -s | tr '[:upper:]' '[:lower:]')"
export PLATFORM_ALIAS="?"
if [[ "$PLATFORM_OS" == "linux"  ]]; then
	PLATFORM_ALIAS="linux"
	if [ -f /etc/os-release ]; then
        source /etc/os-release
        export PLATFORM_OS_RELEASE="$(echo "$ID" | tr '[:upper:]' '[:lower:]')"
    elif [ -f /etc/lsb-release ]; then
        export PLATFORM_OS_RELEASE="$(echo "$DISTRIB_ID" | tr '[:upper:]' '[:lower:]')"
    else
        export PLATFORM_OS_RELEASE="$(uname -o | tr '[:upper:]' '[:lower:]')"
    fi

    case "$PLATFORM_OS_RELEASE" in
        android)
	        PLATFORM_LOGO="󰀲"	
            ;; 
        arch)
            PLATFORM_LOGO="󰣇"
            ;;
        debian)
		    PLATFORM_LOGO=""
            ;;
        manjaro)
		    PLATFORM_LOGO=""
            ;;
        mint)
		    PLATFORM_LOGO="󰣭"
            ;;
        fedora)
		    PLATFORM_LOGO=""
            ;;
        opensuse*)
		    PLATFORM_LOGO=""
            ;;
        *)
            PLATFORM_LOGO="?"
            ;;
    esac
elif [[ "$PLATFORM_OS" == "darwin" ]]; then
	PLATFORM_ALIAS="osx"
    PLATFORM_LOGO=""   
elif [[ "$PLATFORM_OS" =~ ^(msys|cygwin|mingw) ]]; then
	PLATFORM_ALIAS="mswin"
	PLATFORM_LOGO=""

	alias tmux='script -c tmux /dev/null'
	alias msbuild=msbuild.exe
fi

if [[ $UID -eq 0 ]]; then
    export PS1="%F{green}┌─%F{green}(%B%F{blue}$PLATFORM_LOGO%b%F{green})%F{green}──%F{green}[%B%F{blue}%n%F{reset}%b@%B%F{blue}%m%F{green}%b]%F{green}─%F{green}[%B%F{yellow}%~%F{green}%b]%F{reset}"$'\n'"%F{green}└─%F{yellow}%B#%F{reset}%b: "
else
    export PS1="%F{green}┌─%F{green}(%B%F{blue}$PLATFORM_LOGO%b%F{green})%F{green}──%F{green}[%B%F{blue}%n%F{reset}%b@%B%F{blue}%m%F{green}%b]%F{green}─%F{green}[%B%F{yellow}%~%F{green}%b]%F{reset}"$'\n'"%F{green}└─%F{blue}%B$%F{reset}%b: "
fi

if [[ "$PLATFORM_OS" == "darwin" ]]; then
    export PATH="/usr/local/opt/llvm/bin:$PATH"
    export LDFLAGS="-L/usr/local/opt/llvm/lib"
    export CPPFLAGS="-I/usr/local/opt/llvm/include"
    export DOTNET_ROOT="/usr/local/opt/dotnet/libexec"
fi

# Im env varery
#alias emacs="emacsclient -c -a emacs"
alias ed='emacsclient -nw -c'
alias icpy='xsel --clipboard --input'
alias logoff='sudo systemctl restart sddm'
alias doom="$HOME/.emacs.d/bin/doom"
alias remotedekstop='x0vncserver -PasswordFile=/home/loghost/.vnc/passwd -AlwaysShared -AcceptPointerEvents=false AcceptKeyEvents=false SecurityTypes=None'
alias open=xdg-open
export EDITOR=lvim
alias vim=lvim

# Tell ld to also look into ./ or ./lib for when dynamically linking.
export LD_LIBRARY_PATH=./:./lib:$LD_LIBRARY_PATH

# If you come from bash you might have to change your $PATH.
export PATH=$HOME/bin:/usr/local/bin:~/scripts:$PATH:$HOME/.local/bin
export TERMINAL=konsole

# Path to your oh-my-zsh installation.
export ZSH="$HOME/.oh-my-zsh"
export ZSH_PLUGINS="$ZSH/custom/plugins"

# Check if the plugins directory exists, and create it if not
if [[ ! -d "$ZSH_PLUGINS" ]]; then
    echo "[!]: Creating the Zsh plugins directory..."
        mkdir -p "$ZSH_PLUGINS"
fi

download_plugin() {
    local plugin_url="$1"
    local plugin_name="$2"
    local plugin_path="$ZSH_PLUGINS/$plugin_name"

    if [[ ! -d "$plugin_path" ]]; then
        echo "[!]: Downloading $plugin_name..."
        git clone "$plugin_url" "$plugin_path"
        echo "[!]: Downloaded $plugin_name"
    fi
}

# Define the plugin URLs
autosuggestions_url="https://github.com/zsh-users/zsh-autosuggestions"
highlighting_url="https://github.com/zsh-users/zsh-syntax-highlighting"
fzf_url="https://github.com/junegunn/fzf"

# Download zsh-autosuggestions
download_plugin "$autosuggestions_url" "zsh-autosuggestions"

# Download zsh-syntax-highlighting
download_plugin "$highlighting_url" "zsh-syntax-highlighting"

# Download fzf
download_plugin "$fzf_url" "fzf"
if [[ ! -d "$HOME/.fzf" ]]; then
    echo "[!]: Installing fzf..."
    git clone --depth 1 "$fzf_url" "$HOME/.fzf"
    "$HOME/.fzf/install"
    echo "Installed fzf."
fi

# Set name of the theme to load --- if set to "random", it will
# load a random theme each time oh-my-zsh is loaded, in which case,
# to know which specific one was loaded, run: echo $RANDOM_THEME
# See https://github.com/ohmyzsh/ohmyzsh/wiki/Themes
#ZSH_THEME="robbyrussell"
#ZSH_THEME="powerlevel10k/powerlevel10k"

# Set list of themes to pick from when loading at random
# Setting this variable when ZSH_THEME=random will cause zsh to load
# a theme from this variable instead of looking in $ZSH/themes/
# If set to an empty array, this variable will have no effect.
# ZSH_THEME_RANDOM_CANDIDATES=( "robbyrussell" "agnoster" )

# Uncomment the following line to use case-sensitive completion.
CASE_SENSITIVE="true"

# Uncomment the following line to use hyphen-insensitive completion.
# Case-sensitive completion must be off. _ and - will be interchangeable.
# HYPHEN_INSENSITIVE="true"

# Uncomment one of the following lines to change the auto-update behavior
# zstyle ':omz:update' mode disabled  # disable automatic updates
# zstyle ':omz:update' mode auto      # update automatically without asking
# zstyle ':omz:update' mode reminder  # just remind me to update when it's time

# Uncomment the following line to change how often to auto-update (in days).
# zstyle ':omz:update' frequency 13

# Uncomment the following line if pasting URLs and other text is messed up.
# DISABLE_MAGIC_FUNCTIONS="true"

# Uncomment the following line to disable colors in ls.
# DISABLE_LS_COLORS="true"

# Uncomment the following line to disable auto-setting terminal title.
# DISABLE_AUTO_TITLE="true"

# Uncomment the following line to enable command auto-correction.
ENABLE_CORRECTION="true"

# Uncomment the following line to display red dots whilst waiting for completion.
# You can also set it to another string to have that shown instead of the default red dots.
# e.g. COMPLETION_WAITING_DOTS="%F{yellow}waiting...%f"
# Caution: this setting can cause issues with multiline prompts in zsh < 5.7.1 (see #5765)
# COMPLETION_WAITING_DOTS="true"

# Uncomment the following line if you want to disable marking untracked files
# under VCS as dirty. This makes repository status check for large repositories
# much, much faster.
# DISABLE_UNTRACKED_FILES_DIRTY="true"

# Uncomment the following line if you want to change the command execution time
# stamp shown in the history command output.
# You can set one of the optional three formats:
# "mm/dd/yyyy"|"dd.mm.yyyy"|"yyyy-mm-dd"
# or set a custom format using the strftime function format specifications,
# see 'man strftime' for details.
# HIST_STAMPS="mm/dd/yyyy"

# Would you like to use another custom folder than $ZSH/custom?
# ZSH_CUSTOM=/path/to/new-custom-folder

# Which plugins would you like to load?
# Standard plugins can be found in $ZSH/plugins/
# Custom plugins may be added to $ZSH_CUSTOM/plugins/
# Example format: plugins=(rails git textmate ruby lighthouse)
# Add wisely, as too many plugins slow down shell startup.
plugins=(git jump zsh-autosuggestions zsh-syntax-highlighting)

source $ZSH/oh-my-zsh.sh

# User configuration

# export MANPATH="/usr/local/man:$MANPATH"

# You may need to manually set your language environment
# export LANG=en_US.UTF-8

# Preferred editor for local and remote sessions
# if [[ -n $SSH_CONNECTION ]]; then
#   export EDITOR='vim'
# else
#   export EDITOR='mvim'
# fi

# Compilation flags
#export ARCHFLAGS="-arch x86_64"

# Set personal aliases, overriding those provided by oh-my-zsh libs,
# plugins, and themes. Aliases can be placed here, though oh-my-zsh
# users are encouraged to define aliases within the ZSH_CUSTOM folder.
# For a full list of active aliases, run `alias`.
#
# Example aliases
# alias zshconfig="mate ~/.zshrc"
# alias ohmyzsh="mate ~/.oh-my-zsh"


# Arch Linux command-not-found support, you must have package pkgfile installed
# https://wiki.archlinux.org/index.php/Pkgfile#.22Command_not_found.22_hook
#[[ -e /usr/share/doc/pkgfile/command-not-found.zsh ]] && source /usr/share/doc/pkgfile/command-not-found.zsh

## Options section
setopt correct                                                  # Auto correct mistakes
setopt extendedglob                                             # Extended globbing. Allows using regular expressions with *
setopt nocaseglob                                               # Case insensitive globbing
setopt rcexpandparam                                            # Array expension with parameters
setopt nocheckjobs                                              # Don't warn about running processes when exiting
setopt numericglobsort                                          # Sort filenames numerically when it makes sense
setopt nobeep                                                   # No beep
setopt appendhistory                                            # Immediately append history instead of overwriting
setopt histignorealldups                                        # If a new command is a duplicate, remove the older one
setopt autocd                                                   # if only directory path is entered, cd there.
setopt auto_pushd
setopt pushd_ignore_dups
setopt pushdminus

# Completion.
autoload -Uz compinit
compinit
zstyle ':completion:*' matcher-list 'm:{a-zA-Z}={A-Za-z}'       # Case insensitive tab completion
zstyle ':completion:*' rehash true                              # automatically find new executables in path 
zstyle ':completion:*' list-colors "${(s.:.)LS_COLORS}"         # Colored completion (different colors for dirs/files/etc)
zstyle ':completion:*' completer _expand _complete _ignored _approximate
zstyle ':completion:*' menu select
zstyle ':completion:*' select-prompt '%SScrolling active: current selection at %p%s'
zstyle ':completion:*:descriptions' format '%U%F{cyan}%d%f%u'

# Speed up completions
zstyle ':completion:*' accept-exact '*(N)'
zstyle ':completion:*' use-cache on
zstyle ':completion:*' cache-path ~/.cache/zcache

# automatically load bash completion functions
autoload -U +X bashcompinit && bashcompinit

[ -f ~/.fzf.zsh ] && source ~/.fzf.zsh
