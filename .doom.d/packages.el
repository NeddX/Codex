;; -*- no-byte-compile: t; -*-
;;; $DOOMDIR/packages.el

;; To install a package with Doom you must declare them here and run 'doom sync'
;; on the command line, then restart Emacs for the changes to take effect -- or
;; use 'M-x doom/reload'.


;; To install SOME-PACKAGE from MELPA, ELPA or emacsmirror:
                                        ;(package! some-package)

;; To install a package directly from a remote git repo, you must specify a
;; `:recipe'. You'll find documentation on what `:recipe' accepts here:
;; https://github.com/radian-software/straight.el#the-recipe-format
                                        ;(package! another-package
                                        ;  :recipe (:host github :repo "username/repo"))

;; If the package you are trying to install does not contain a PACKAGENAME.el
;; file, or is located in a subdirectory of the repo, you'll need to specify
;; `:files' in the `:recipe':
                                        ;(package! this-package
                                        ;  :recipe (:host github :repo "username/repo"
                                        ;           :files ("some-file.el" "src/lisp/*.el")))

;; If you'd like to disable a package included with Doom, you can do so here
;; with the `:disable' property:
                                        ;(package! builtin-package :disable t)

;; You can override the recipe of a built in package without having to specify
;; all the properties for `:recipe'. These will inherit the rest of its recipe
;; from Doom or MELPA/ELPA/Emacsmirror:
                                        ;(package! builtin-package :recipe (:nonrecursive t))
                                        ;(package! builtin-package-2 :recipe (:repo "myfork/package"))

;; Specify a `:branch' to install a package from a particular branch or tag.
;; This is required for some packages whose default branch isn't 'master' (which
;; our package manager can't deal with; see radian-software/straight.el#279)
                                        ;(package! builtin-package :recipe (:branch "develop"))

;; Use `:pin' to specify a particular commit to install.
                                        ;(package! builtin-package :pin "1a2b3c4d5e")


;; Doom's packages are pinned to a specific commit and updated from release to
;; release. The `unpin!' macro allows you to unpin single packages...
                                        ;(unpin! pinned-package)
;; ...or multiple packages
                                        ;(unpin! pinned-package another-pinned-package)
;; ...Or *all* packages (NOT RECOMMENDED; will likely break things)
                                        ;(unpin! t)


;;(use-package auto-complete
;;  :ensure t
;;  :init
;;  (progn
;;    (ac-config-default)
;;    (global-auto-complete-mode t)
;;    ))

(package! doom-themes)
(package! lsp-mode)
(package! tree-sitter)
(package! tree-sitter-langs)
(package! treemacs-icons-dired)
(package! treemacs-all-the-icons)
(package! all-the-icons)
(package! all-the-icons-dired)
(package! all-the-icons-completion)
(package! evil-multiedit)
(package! treemacs)
(package! lsp-treemacs)
(package! nerd-icons)
(package! treemacs-evil)
(package! treemacs-persp)
(package! treemacs-magit)
(package! treemacs-tab-bar)
(package! treemacs-nerd-icons)
(package! treemacs-all-the-icons)
(package! treemacs-projectile)
(package! treemacs-icons-dired)
(package! multi-vterm)
(package! lsp-treemacs)
(package! dap-mode)
(package! one-themes)
(package! atom-one-dark-theme)
(package! spacemacs-theme)
;;(package! chess)
(package! pacmacs)
;;(package! good-scroll)
(package! company-lsp)
(package! company-tabnine)
(package! info-colors)
(package! suggest)
(package! nyan-mode)
;;(package! elcord)
;;(package! fast-scroll)
(package! dashboard)
;;(package! rustic)
;;(package! beacon)
(package! clippy)
(package! drag-stuff)
(package! flycheck)
;;(package! emojify) ;; Slow?
;;(package! flycheck-clang-tidy)

;; Disabled packages
(package! eldoc :disable t)
