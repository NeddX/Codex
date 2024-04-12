#!/usr/bin/env python3

import os
import re
import sys
import time
import shutil
import platform
from enum import Enum
import com

def create_folder_structure(src_path):
    if not os.path.exists(src_path):
        os.makedirs(src_path)
        os.makedirs(os.path.join(src_path, 'src'))
        os.makedirs(os.path.join(src_path, 'include'))
        os.makedirs(os.path.join(src_path, 'vendor'))

def copy_files(src_folder, dest_folder):
    for root, dirs, files in os.walk(src_folder):
        for directory in dirs:
            dest_dir = os.path.join(dest_folder, os.path.relpath(os.path.join(root, directory), src_folder))
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
        for file in files:
            if not file.endswith(('.cpp', '.c', '.cc', '.cxx')):
                src_file = os.path.join(root, file)
                dest_file = os.path.join(dest_folder, os.path.relpath(src_file, src_folder))
                shutil.copy2(src_file, dest_file)

def main():
    SRC = "./Codex/src"
    DEST = "./CodexEditor/assets/Projects/TemplateProject"

    create_folder_structure(DEST)
    create_folder_structure(os.path.join(DEST, 'src'))
    create_folder_structure(os.path.join(DEST, 'vendor'))
    create_folder_structure(os.path.join(DEST, 'Platform'))
    create_folder_structure(os.path.join(DEST, 'include'))
    create_folder_structure(os.path.join(DEST, 'include'))

    SRC = './CodexEditor/assets/Projects/TemplateProject/Engine'
    E_DEST = './Codex/src'
    V_DEST = os.path.join(SRC, 'vendor')
    P_DEST = os.path.join(SRC, 'Platform')

    com.log(f':: Copying minimal engine headers to: {E_DEST}')

    create_folder_structure(SRC)
    create_folder_structure(os.path.join(SRC, 'src'))
    create_folder_structure(os.path.join(SRC, 'vendor'))
    create_folder_structure(os.path.join(SRC, 'Platform'))

    copy_files(E_DEST, SRC)
    copy_files(V_DEST, SRC)
    copy_files(P_DEST, SRC)

if __name__ == '__main__':
    main()
