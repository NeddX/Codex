((nil . ((eval . (add-hook 'before-save-hook
                           (lambda ()
                             (when (and (bound-and-true-p lsp-mode)
                                        (fboundp 'lsp-format-buffer))
                               (lsp-format-buffer)))
                           nil t))
         (projectile-project-compilation-cmd . "Scripts/build.py --build --install --config=debug")
         (eval . (progn
                   (setq +format-on-save-enabled-modes t))))))
