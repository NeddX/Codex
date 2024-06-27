#!/usr/bin/env python3

# Tiny script that will just copy the Minimal Engine headers to the
# template project so that I don't have to do too much CMake scripting.
# This script will be called from CMake.

import os
import re
import sys
import time
import shutil
import platform
from enum import Enum
import com

args = sys.argv

SRC = None
DEST = None
ROOT_DIR = None
BINARY_DIR = None
HEADERS_EXT_LIST = ('.h', '.hpp', '.inl')

if len(args) >= 2:
    for arg in args:
        if arg.startswith('--root-dir='):
            ROOT_DIR = arg.split('=')[1]
        elif arg.startswith('--binary-dir='):
            BINARY_DIR = arg.split('=')[1]
        elif arg.startswith('--src-dir='):
            SRC = arg.split('=')[1]
        elif arg.startswith('--dest-dir='):
            DEST = arg.split('=')[1]

def create_folder_structure(src_path):
    if not os.path.exists(src_path):
        os.makedirs(src_path)

def copy_files(src_folder, dest_folder, ext_list = None):
    com.log(f'Copying files from: {src_folder} to: {dest_folder} with extensions: {ext_list}')
    for root, dirs, files in os.walk(src_folder):
        for directory in dirs:
            dest_dir = os.path.join(dest_folder, os.path.relpath(os.path.join(root, directory), src_folder))
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
        for file in files:
            if ext_list == None or file.endswith(ext_list):
                src_file = os.path.join(root, file)
                dest_file = os.path.join(dest_folder, os.path.relpath(src_file, src_folder))
                shutil.copy2(src_file, dest_file)

def main():
    com.log('Copying minimal engine headers...')

    create_folder_structure(DEST)

    # Engine folders.
    create_folder_structure(os.path.join(DEST, 'Engine/include'))
    create_folder_structure(os.path.join(DEST, 'Engine/src'))
    create_folder_structure(os.path.join(DEST, 'Engine/vendor'))
    create_folder_structure(os.path.join(DEST, 'Engine/src/Engine'))
    create_folder_structure(os.path.join(DEST, 'Engine/src/PCH'))
    create_folder_structure(os.path.join(DEST, 'Engine/src/Platform'))
    copy_files(os.path.join(SRC, 'include'), os.path.join(DEST, 'Engine/include'))
    copy_files(os.path.join(SRC, 'src/Engine'), os.path.join(DEST, 'Engine/src/Engine'), HEADERS_EXT_LIST)
    copy_files(os.path.join(SRC, 'src/PCH'), os.path.join(DEST, 'Engine/src/PCH'), HEADERS_EXT_LIST)
    copy_files(os.path.join(SRC, 'src/Platform'), os.path.join(DEST, 'Engine/src/Platform'), HEADERS_EXT_LIST)
    copy_files(os.path.join(SRC, 'vendor'), os.path.join(DEST, 'Engine/vendor'), HEADERS_EXT_LIST)

    # Scripts.
    create_folder_structure(os.path.join(DEST, 'Scripts'))
    copy_files(os.path.join(ROOT_DIR, 'Scripts'), os.path.join(DEST, 'Scripts'))

if __name__ == '__main__':
    main()
