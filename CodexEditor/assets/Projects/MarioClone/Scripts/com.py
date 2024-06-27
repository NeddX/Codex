#!/usr/bin/env python3

import os
import re
import sys
import time
import shutil
import ctypes
import platform
from enum import Enum
import subprocess as sb

class Chrono:
    tp = None
    @staticmethod
    def begin():
        Chrono.tp = time.time()

    @staticmethod
    def end():
        if Chrono.tp is None:
            raise ValueError('Called Chrono.end() before calling Chrono.begin()')
        elapsed = time.time() - Chrono.tp
        tp = None
        return elapsed * 1000

def log(msg):
    print(f'[Build] [Info] :: {msg}')

def warn(msg):
    print(f'[Build] [Warning] :: {msg}')

def err(msg):
    print(f'[Build] [Panic] :: {msg}')

def panic(msg, exit_code = 1):
    err(msg)
    sys.exit(exit_code)

def run(cmd, shell = True, stdout = None, stderr = None, capture_output = False, text = None):
    return sb.run(cmd, shell=shell, stdout=stdout, stderr=stderr, capture_output=capture_output, text=text)

def get_cmake_presets():
    res = run('cmake --list-presets', shell=True, stdout=sb.PIPE)
    if res:
        output = res.stdout.decode('utf-8')
        if output == '':
            return []
    else:
        return []

    # Pythonic autism
    presets = output.replace(' ', '').split('\n')
    presets = presets[2:-1]
    presets = [(s[:s.rfind('"')])[1:] for s in presets]

    return presets

def win32_is_admin():
    try:
        return ctypes.windll.shell32.IsUserAdmin()
    except:
        return False

def win32_request_admin():
    ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, " ".join(sys.argv), None, 1)
