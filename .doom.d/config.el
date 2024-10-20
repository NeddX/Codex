;;; $DOOMDIR/config.el -*- lexical-binding: t; -*-

;; Place your private configuration here! Remember, you do not need to run 'doom
;; sync' after modifying this file!


;; Some functionality uses this to identify you, e.g. GPG configuration, email
;; clients, file templates and snippets. It is optional.
(setq! user-full-name "Neddiendrohu"
       user-mail-address "donskinler@gmail.com")

;;
;; Doom exposes five (optional) variables for controlling fonts in Doom:
;; - `doom-font' -- the primary font to use
;; - `doom-variable-pitch-font' -- a non-monospace font (where applicable)
;; - `doom-big-font' -- used for `doom-big-font-mode'; use this for
;;   presentations or streaming.
;; - `doom-unicode-font' -- for unicode glyphs
;; - `doom-serif-font' -- for the `fixed-pitch-serif' face
;;
;; See 'C-h v doom-font' for documentation and more examples of what they
;; accept. For example:
;;
;;(setq! doom-font (font-spec :family "Fira Code" :size 12 :weight 'semi-light)
;;      doom-variable-pitch-font (font-spec :family "Fira Sans" :size 13))
;;
;; If you or Emacs can't find your font, use 'M-x describe-font' to look them
;; up, `M-x eval-region' to execute elisp code, and 'M-x doom/reload-font' to
;; refresh your font settings. If Emacs still can't find your font, it likely
;; wasn't installed correctly. Font issues are rarely Doom issues!

;; There are two ways to load a theme. Both assume the theme is installed and
;; available. You can either set `doom-theme' or manually load a theme with the
;; `load-theme' function. This is the default:
;;(setq! doom-theme 'doom-one)

;; This determines the style of line numbers in effect. If set to `nil', line
;; numbers are disabled. For relative line numbers, set this to `relative'.
(setq! display-line-numbers-type t)

;; If you use `org' and don't want your org files in the default location below,
;; change `org-directory'. It must be set before org loads!
(setq! org-directory "~/org/")


;; Whenever you reconfigure a package, make sure to wrap your config in an
;; `after!' block, otherwise Doom's defaults may override your settings. E.g.
;;
;;   (after! PACKAGE
;;     (setq! x y))
;;
;; The exceptions to this rule:
;;
;;   - Setting file/directory variables (like `org-directory')
;;   - Setting variables which explicitly tell you to set them before their
;;     package is loaded (see 'C-h v VARIABLE' to look up their documentation).
;;   - Setting doom variables (which start with 'doom-' or '+').
;;
;; Here are some additional functions/macros that will help you configure Doom.
;;
;; - `load!' for loading external *.el files relative to this one
;; - `use-package!' for configuring packages
;; - `after!' for running code after a package has loaded
;; - `add-load-path!' for adding directories to the `load-path', relative to
;;   this file. Emacs searches the `load-path' when you load packages with
;;   `require' or `use-package'.
;; - `map!' for binding new keys
;;
;; To get information about any of these functions/macros, move the cursor over
;; the highlighted symbol at press 'K' (non-evil users must press 'C-c c k').
;; This will open documentation for it, including demos of how they are used.
;; Alternatively, use `C-h o' to look up a symbol (functions, variables, faces,
;; etc).
;;
;; You can also try 'gd' (or 'C-c c d') to jump to their definition and see how
;; they are implemented.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Doom/Emacs (Global)
                                        ;(setq! doom-debug-mode t)
(setq! fancy-splash-image (concat doom-user-dir "assets/splash.png"))
                                        ;(setq! doom-font (font-spec :family "Hack Nerd Font Mono" :size 16))
(setq! doom-font (font-spec :family "JetBrainsMono Nerd Font Mono" :size 16))
(setq! scroll-step 1)
(setq! scroll-margin 8)
(setq! scroll-conservatively 10000)
(setq! inhibit-compacting-font-caches t)
(setq! evil-shift-width 4)
(setq! evil-indent-convert-tabs nil)
(setq! indent-tabs-mode nil)
(setq! undo-limit 80000000)
(setq! confirm-kill-emacs nil)
(after! doom-themes
  (setq! doom-themes-enable-bold t
         doom-themes-enable-italic t))
;;(setq! all-the-icons-completion-mode t)
(setq! better-gc-cons-threshold (* 700 1024 1024))
                                        ; Set the GC threshold to 700 MB (becasue garbage collected languages first need to collect themselves out of this world)
(setq! gc-cons-threshold better-gc-cons-threshold)
(setq! read-process-output-max (* 2 1024 1024)) ;; 2mb
(setq! evil-want-fine-undo t)
(setq! display-line-numbers-type 't)
(setq! display-line-numbers-widen t)
(setq! inhibit-compacting-font-caches t)
(setq! find-file-visit-truename t)
;;(defvar better-gc-cons-threshold 134217728) ; 128mb
;; Consider the following (WARNING!: SLOWEST MODULES)
;; :ui indent-guides
;; :ui ligatures
;; :ui

                                        ; Set the current theme
;; Cool themes
;;(setq! doom-theme 'doom-one)
;;(setq! doom-theme 'doom-tomorrow-night)
;;(setq! doom-theme 'doom-tokyo-night)
;;(setq! doom-theme 'atom-one-dark)
;;(setq! doom-theme 'doom-horizon)
;;(setq! doom-theme 'doom-ayu-dark) ;; Second favourite
;;(setq! doom-theme 'doom-old-hope)
;;(setq! doom-theme 'doom-vivendi)
;;(setq! doom-theme 'doom-acario-dark)
;;
;;(setq! doom-theme 'doom-ir-black) ;; Third favourite
(setq! doom-theme 'doom-meltbus) ;; First favourite

                                        ; Custom syntax highlighting for doom-meltbus
(if (equal doom-theme 'doom-meltbus)
    (progn
      (custom-set-faces!
        '(font-lock-keyword-face :foreground "#eb2672")
        '(font-lock-function-name-face :foreground "#60d7db")
        '(font-lock-type-face :foreground "#78c241")
        '(font-lock-operator-face :foreground "#bf984a") ; Fix typo here
        '(font-lock-constant-face :foreground "#57d0e9")
        '(show-paren-match-face :foreground "#78c241")
        '(font-lock-string-face :foreground "#bf984a")
        '(font-lock-constant-face :foreground "#99609c")
        '(font-lock-namespace-face :foreground "#858585")
        '(font-lock-comment-face :slant italic)
        '(font-lock-variable-name-face :foreground "#bc9334")
        '(cursor :background "#eb2672")
        ;;'(hl-line :foreground "#eb2672")
        )
      )
  )

;; WE WILL INVOKE THE GARBAGE COLLECTOR OURSELVES!!
(add-hook 'emacs-startup-hook
          (lambda ()
            (if (boundp 'after-focus-change-function)
                (add-function :after after-focus-change-function
                              (lambda ()
                                (unless (frame-focus-state)
                                  (garbage-collect))))
              (add-hook 'after-focus-change-function 'garbage-collect))
            (defun gc-minibuffer-setup-hook ()
              (setq! gc-cons-threshold (* better-gc-cons-threshold 2)))

            (defun gc-minibuffer-exit-hook ()
              (garbage-collect)
              (setq! gc-cons-threshold better-gc-cons-threshold))

            (add-hook 'minibuffer-setup-hook #'gc-minibuffer-setup-hook)
            (add-hook 'minibuffer-exit-hook #'gc-minibuffer-exit-hook)))

;; Beacon mode
(use-package beacon
  :defer t
  :config
  (beacon-mode t))

;; Centaur
(use-package! centaur-tabs
  :ensure t
  :init
  (add-hook! 'server-after-make-frame-hook 'centaur-tabs-mode)
  ;;(doom-add! 'hook-after-init-hook #'centaur-tabs-mode)
  :config
  (global-set-key (kbd "<C-S-left>")  'centaur-tabs-backward)
  (global-set-key (kbd "<C-S-right>") 'centaur-tabs-forward)
  (global-set-key (kbd "<S-left>") 'centaur-tabs-move-current-tab-to-left)
  (global-set-key (kbd "<S-right>") 'centaur-tabs-move-current-tab-to-right)
  (global-set-key (kbd "C-S-n") 'centaur-tabs--create-new-empty-buffer)
  (global-set-key (kbd "C-S-w") 'centaur-tabs--kill-this-buffer-dont-ask)
  (setq!
   centaur-tabs-cycle-scope 'tabs
   centaur-tabs-style "alternate"
   centaur-tabs-height 32
   centaur-tabs-set-icons t
   centaur-tabs-gray-out-icons 'buffer
   centaur-tabs-set-bar 'over) ;; 'left doesn't work with emacsclient for some fucking reason
  (centaur-tabs-group-by-projectile-project)
  (centaur-tabs-headline-match)
  (centaur-tabs-mode t)
  )

;; Toggle centaur-tabs-mode based on the visibility of '*dashboard*' buffer."
(defun my-toggle-centaur-tabs-mode ()
  (if (get-buffer-window "*dashboard*" 'visible)
      (centaur-tabs-mode -1)
    (centaur-tabs-mode 1)))

(add-hook 'buffer-list-update-hook 'my-toggle-centaur-tabs-mode)

(defun toggle-centaur-tabs ()
  (if (string= (buffer-name) "*dashboard*")
      (centaur-tabs-mode -1) ; Disable centaur-tabs
    (centaur-tabs-mode 1))) ; Enable centaur-tabs

;;(add-hook 'buffer-list-update-hook 'toggle-centaur-tabs)

;; Org
(after! org-mode
  (org +pretty)
  (setq! org-ellipsis " ▾ ")
  (appendq! +ligatures-extra-symbols
            `(:checkbox      "☐"
              :pending       "◼"
              :checkedbox    "☑"
              :list_property "∷"
              :em_dash       "—"
              :ellipses      "…"
              :arrow_right   "→"
              :arrow_left    "←"
              :begin_quote   "❝"
              :end_quote     "❞"
              :header        "›"
              ;;:priority_a   ,(propertize "⚑" 'face 'all-the-icons-red)
              ;;:priority_b   ,(propertize "⬆" 'face 'all-the-icons-orange)
              ;;:priority_c   ,(propertize "■" 'face 'all-the-icons-yellow)
              ;;:priority_d   ,(propertize "⬇" 'face 'all-the-icons-green)
              ;;:priority_e   ,(propertize "❓" 'face 'all-the-icons-blue)
              :roam_tags nil
              :filetags nil))
  (set-ligatures! 'org-mode
    :merge t
    :checkbox      "[ ]"
    :pending       "[-]"
    :checkedbox    "[x]"
    :list_property "::"
    :em_dash       "---"
    :ellipsis      "..."
    :arrow_right   "->"
    :arrow_left    "<-"
    :title         "#+title:"
    :subtitle      "#+subtitle:"
    :author        "#+author:"
    :date          "#+date:"
    :property      "#+property:"
    :options       "#+options:"
    :startup       "#+startup:"
    :macro         "#+macro:"
    :html_head     "#+html_head:"
    :html          "#+html:"
    :latex_class   "#+latex_class:"
    :latex_header  "#+latex_header:"
    :beamer_header "#+beamer_header:"
    :latex         "#+latex:"
    :attr_latex    "#+attr_latex:"
    :attr_html     "#+attr_html:"
    :attr_org      "#+attr_org:"
    :begin_quote   "#+begin_quote"
    :end_quote     "#+end_quote"
    :caption       "#+caption:"
    :header        "#+header:"
    :begin_export  "#+begin_export"
    :end_export    "#+end_export"
    :results       "#+RESULTS:"
    :property      ":PROPERTIES:"
    :end           ":END:"
    :priority_a    "[#A]"
    :priority_b    "[#B]"
    :priority_c    "[#C]"
    :priority_d    "[#D]"
    :priority_e    "[#E]"
    :roam_tags     "#+roam_tags:"
    :filetags      "#+filetags:")
  (plist-put +ligatures-extra-symbols :name "⁍"))

(defun my-org-mode-hook ()
  (define-key org-mode-map (kbd "<M-S-up>") nil)
  (define-key org-mode-map (kbd "<M-S-down>") nil)
  (define-key org-mode-map (kbd "<M-up>") nil)
  (define-key org-mode-map (kbd "<M-down>") nil)
  (define-key org-mode-map (kbd "<C-S-left>") nil)
  (define-key org-mode-map (kbd "<C-S-right>") nil)
  (define-key org-mode-map (kbd "<M-left>") nil)
  (define-key org-mode-map (kbd "<S-left>") nil)
  (define-key org-mode-map (kbd "<S-right>") nil)
  (define-key org-mode-map (kbd "<S-up>") nil)
  (define-key org-mode-map (kbd "<S-down>") nil)
  (define-key org-mode-map (kbd "<M-right>") nil)
  (define-key org-mode-map (kbd "C-<tab>") nil)
  (define-key org-mode-map (kbd "C-S-<tab>") nil)
  (define-key org-mode-map (kbd "<C-up>") nil)
  (define-key org-mode-map (kbd "<C-down>") nil))

(add-hook 'org-mode-hook 'my-org-mode-hook)

;; dashboard
(use-package dashboard
  :ensure t
  :bind (:map dashboard-mode-map
              ;; ("j" . nil)
              ;; ("k" . nil)
              ("n" . 'dashboard-next-line)
              ("p" . 'dashboard-previous-line)
              )
  :init (add-hook 'dashboard-mode-hook (lambda () (setq! show-trailing-whitespace nil)))
  :custom
  (dashboard-set-navigator t)
  (dashboard-center-content t)
  (dashboard-set-file-icons t)
  (dashboard-set-heading-icons t)
  (dashboard-image-banner-max-height 250)
  (dashboard-banner-logo-title "[EMACS is my operating system]")
  (dashboard-startup-banner (concat doom-user-dir "assets/splash.png"))
  :config
  (setq! doom-fallback-buffer-name "*dashboard*")
  (dashboard-setup-startup-hook)
  (setq! dashboard-footer-icon (nerd-icons-codicon "nf-cod-calendar"
                                                   :height 1.1
                                                   :v-adjust -0.05
                                                   :face 'font-lock-keyword-face))

  (setq! dashboard-navigator-buttons
         `(;; line1
           ((,(nerd-icons-codicon "nf-cod-octoface" :height 1.5 :v-adjust 0.0)
             "GitHub"
             "Browse your github... nedd..."
             (lambda (&rest _) (browse-url "https://github.com/NeddX")) nil "" " |")
            (,(nerd-icons-codicon "nf-cod-refresh" :height 1.5 :v-adjust 0.0)
             "Update"
             "Update Emacs"
             (lambda (&rest _) (auto-package-update-maybe)) warning "" " "))
           ;;(,(nerd-icons-faicon "nf-fa-flag" :height 1.5 :v-adjust 0.0) nil
           ;; "Report a BUG"
           ;;(lambda (&rest _) (browse-url "https://github.com/Likhon-baRoy/.emacs.d/issues/new")) error "" ""))

           ;; line 2
           ;; ((,(all-the-icons-octicon "mark-github" :height 1.1 :v-adjust 0.0)
           ;;   "AlienFriend"
           ;;   "Browse Alien Page"
           ;;   (lambda (&rest _) (browse-url "https://github.com/b-coimbra/.emacs.d")) nil "" ""))
           ;; Empty line
           (("" "\n" "" nil nil "" ""))

           ;; Keybindings
           ((,(nerd-icons-faicon "nf-fa-search" :height 0.9 :v-adjust -0.1)
             " Find file" nil
             (lambda (&rest _) (interactive) (call-interactively 'find-file)) nil "" "            SPC SPC"))
           ((,(nerd-icons-octicon "nf-oct-file_directory" :height 1.0 :v-adjust -0.1)
             " Open project" nil
             (lambda (&rest _) (interactive) (call-interactively 'projectile-switch-project)) nil "" "         SPC p p"))
           ((,(nerd-icons-octicon "nf-oct-three_bars" :height 1.1 :v-adjust -0.1)
             " File explorer" nil
             (lambda (&rest _) (interactive) (call-interactively 'dired)) nil "" "          C-x d"))
           ;;((,(nerd-icons-codicon "nf-cod-settings" :height 0.9 :v-adjust -0.1)
           ;;  " Open settings" nil
           ;;  (lambda (&rest _) (open-config-file)) nil "" "        C-c e  "))
           ))
  (setq!
   dashboard-items '((recents        . 3)
                     (projects       . 3)
                     (bookmarks      . 5)
                     ))
  :custom-face
  (dashboard-heading ((t (:foreground nil :weight bold))))) ; "#f1fa8c"

;; info-colors
(use-package info-colors
  :defer t
  :commands (info-colors-fontify-node))

(add-hook 'Info-selection-hook
          'info-colors-fontify-node)

;; Emojify
(use-package emojify
  :defer t
  :hook (after-init . global-emojify-mode))

;; Nyan mode
(use-package nyan-mode
  :ensure t ; most definetly ensure the cat
  :init
  (if (window-system)
      (nyan-mode))
  :config
  (setq! nyan-animate-nyancat t
         nyan-wavy-trail t))

;; Modeline (the status bar thing)
(after! doom-modeline
  (setq doom-modeline-enable-word-count t
        doom-modeline-persp-name t
        doom-modeline-persp-icon t
        doom-modeline-major-mode-icon t))

;; Doom theme
(after! doom-themes
  (setq! doom-themes-enable-bold t
         doom-themes-enable-italic t))

;; Info colors
(use-package info-colors
  :defer t
  :commands (info-colors-fontify-node))

(add-hook 'Info-selection-hook
          'info-colors-fontify-node)

;;class
;; Syntax highlighting if doom-meltbus

;; Multiedit
(use-package evil-multiedit
  :defer t
  :config
  (evil-multiedit-default-keybinds))


(map! :leader
      :desc "Goto line"
      "f g" #'goto-line)
(map! :map go-mode-map
      :localleader
      "gd" #'lsp-find-definition)
(map!
 :leader
 :desc "Discover projectile projects in current directory."
 "p D" #'projectile-discover-projects-in-directory)
(map!
 :leader
 :desc "Install a package from MELPA"
 "p i" #'package-install)
(map!
 :leader
 :desc "Toggle vterm"
 "o o" #'+vterm/toggle)
(map!
 :leader
 :desc "New multi-vterm instance"
 "o n" #'multi-vterm)
(map!
 :leader
 :desc "Toggle next multi-vterm instance"
 "o t" #'multi-vterm-next)
(map!
 :leader
 :desc "Restart LSP workspace"
 "c R" #'lsp-workspace-restart)
(map!
 :leader
 :desc "Start Dap Debug session"
 "d S" (lambda ()
         (interactive)
         (dap-mode 1)
                                        ;(setq! dap-auto-configure-features nil)
         (map! :map dap-mode-map
               "<f5>" #'dap-continue
               "<f10>" #'dap-next
               "<f11>" #'dap-step-in
               "<S-f11>" #'dap-step-out)
         (call-interactively 'dap-debug-last)
         )
 )
(map!
 :leader
 :desc "Disable Expensive plugins such as Lsp, flycheck, elcord and company cause dogshit laptop :)"
 "l D" (lambda ()
         (interactive)
         (dap-mode 0)
         (lsp-mode 0)
         (flycheck-mode 0)
         (company-mode 0)
         ;;(elcord-mode 0)
         )
 )
(map!
 :leader
 :desc "Stop Dap Debug session"
 "d k" (lambda ()
         (interactive)
         (dap-mode 0)
         (call-interactively 'dap-disconnect)
         )
 )
(map!
 :leader
 :desc "Delete saved workspace"
 "TAB D" #'lsp-workspace-folders-remove)
                                        ;(setq dap-ui-buffer-configurations
                                        ;      '((,"*DAP Debug Output*"
                                        ;         (side . bottom)
                                        ;         (slot . 1)
                                        ;         (window-width . 0.20)
                                        ;         (window-height . 0.20))
                                        ;        (,"*DAP Locals*"
                                        ;         (side . right)
                                        ;         (slot . 1)
                                        ;         (window-width . 0.20)
                                        ;         (window-height . 0.50))
                                        ;        (,"*DAP Call Stack*"
                                        ;         (side . right)
                                        ;         (slot . 2)
                                        ;         (window-width . 0.20)
                                        ;         (window-height . 0.50)))))

(map!
 :leader
 :desc "Add breakpoint at the current line"
 "d b" #'dap-breakpoint-add)
(map!;
 :leader
 :desc "Remove breakpoint at the current line"
 "d r" #'dap-breakpoint-delete)
(map!
 :leader
 :desc "Toggle breakpoint at the current line"
 "d t" #'dap-breakpoint-toggle)
(map!
 :leader
 :desc "Delete all breakpoints"
 "d R" #'dap-breakpoint-delete-all)

                                        ;(use-package! dap-ui
                                        ;  :ensure t
                                        ;  :config
                                        ;  ;; Set the layout for the DAP-Debug windows
                                        ;  (setq! dap-ui-buffer-configurations
                                        ;        '((sidebar . ((side . right) (slot . 1) (window-width . 0.35)))
                                        ;          (locals . ((side . right) (slot . 2) (window-width . 0.35)))
                                        ;          (sessions . ((side . left) (slot . 1) (window-width . 0.35)))
                                        ;          (breakpoints . ((side . left) (slot . 2) (window-width . 0.35)))
                                        ;          (debug . ((side . bottom) (slot . 3) (window-height . 0.4) (window-width . 0.35)))))

;; Treesitter
(use-package tree-sitter
  :defer t
  :hook (prog-mode . turn-on-tree-sitter-mode)
  :hook (tree-sitter-after-on . tree-sitter-hl-mode)
  :config
  (require 'tree-sitter-langs)
  (setq! tree-sitter-debug-jump-buttons t
         tree-sitter-debug-highlight-jump-region t))

;; Dired
;;(add-hook 'dired-mode-hook 'all-the-icons-dired-mode)

;; Treemacs
(use-package treemacs
  :defer t
  :config
  (setq! doom-themes-treemacs-theme "doom-colors")
  (treemacs-project-follow-mode 1))

                                        ; Dap
(use-package dap-mode
  :defer t
  :init
  (dap-mode 0)
  :config
  (dap-ui-mode)
  (dap-ui-controls-mode 1)
  (require 'dap-cpptools)
  (require 'dap-lldb)
  (require 'dap-gdb-lldb)

  ;; Extensions from vscode
  (dap-gdb-lldb-setup)
  (dap-register-debug-template
   "Rust: LLDB Run Configuration"
   (list :type "lldb"
         :request "launch"
         :name "LLDB:Run"
         :gdpath "rust-lldb"
         :target nil
         :cwd nil))

  :custom
  (dap-auto-configure-features '(locals expressions))
                                        ;(dap-auto-configure-features nil)

  :config
  (dap-ui-mode 1)
  (setq! dap-gdb-lldb-debug-window nil)
  )

;; Elcord
(use-package elcord
  :defer t
  :config
  (elcord-mode 0)) ; no discord for now.

;;(use-package fast-scroll
;; :init
;; (add-hook 'fast-scroll-start-hook (lambda () (flycheck-mode -1)))
;; (add-hook 'fast-scroll-end-hook (lambda () (flycheck-mode 1)))
;; :config
;; (fast-scroll-config)
;; (fast-scroll-mode 1))

;; lsp-ui
(use-package lsp-ui
  :defer t
  :commands lsp-ui-mode
  :config
  (setq! lsp-ui-sideline-enable t)
  :custom
  (lsp-ui-peek-always-show t)
  (lsp-ui-sideline-show-hover t)
  (lsp-ui-doc-enable nil))

;; lsp-mode
(use-package lsp-mode
  :defer t
  :init
  (setq! lsp-enable-symbol-highlighting t
         lsp-lens-enable t
         lsp-headerline-breadcrumb-enable t
         lsp-modeline-code-actions-enable t
         lsp-diagnostics-provider :flycheck
         lsp-completion-show-detail t
         lsp-completion-show-kind t
         lsp-log-io nil ;; You might consider commenting this line if LSP log is required.
         lsp-clients-clangd-args '("--header-insertion=never" "--background-index=false" "--clang-tidy"))
  :custom
  (lsp-eldoc-render-all nil)
  (lsp-idle-delay 0.500)
  (lsp-inlay-hint-enable t)
  ;; These are optional configurations. See https://emacs-lsp.github.io/lsp-mode/page/lsp-rust-analyzer/#lsp-rust-analyzer-display-chaining-hints for a full list
  (lsp-rust-analyzer-cargo-watch-command "clippy")
  (lsp-rust-analyzer-display-lifetime-elision-hints-enable "skip_trivial")
  (lsp-rust-analyzer-display-chaining-hints t)
  (lsp-rust-analyzer-display-lifetime-elision-hints-use-parameter-names nil)
  (lsp-rust-analyzer-display-closure-return-type-hints t)
  (lsp-rust-analyzer-display-parameter-hints nil)
  (lsp-rust-analyzer-display-reborrow-hints nil)
  :config
  (add-hook 'lsp-mode-hook 'lsp-ui-mode))

;; Projectile
(after! projectile
  (setq projectile-project-search-path '("~/projects/" "~/work/" "~/dev" "~")))

;; Company(use-package! company
(add-hook 'after-init-hook 'global-company-mode)
;; from modules/completion/company/config.el
(use-package company
  :defer t
  :commands (company-mode global-company-mode company-complete
                          company-complete-common company-manual-begin company-grab-line)
  :config
  (setq! company-idle-delay 0.3
         company-tooltip-limit 20
         company-dabbrev-downcase nil
         company-dabbrev-ignore-case nil
         company-minimum-prefix-length 1))

;; Drag stuff up/down
(use-package drag-stuff
  :defer t
  :config
  (drag-stuff-mode 1))

(map! "M-<down>" (lambda () (interactive) (drag-stuff-down 1)))
(map! "M-<up>" (lambda () (interactive) (drag-stuff-up 1)))

;; Because my $HOME is a git repo.
(after! projectile (setq projectile-project-root-files-bottom-up (remove ".git"
                                                                         projectile-project-root-files-bottom-up)))


;; Useful stuff
;; SPC-h-k: Describe key.
;; SPC-h-f: Describe function.
;; SPC-h-d-h: Doom documentation.
