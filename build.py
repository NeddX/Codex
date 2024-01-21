#!/usr/bin/env python3

import os
import re
import sys
import time
import shutil
from enum import Enum
import subprocess as sb

class Action:
    Nope = 0
    Build = 1

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

action = Action.Build
preset = None
stdoutput = None

def log(msg):
    print(f'[Build][Info] :: {msg}')

def warn(msg):
    print(f'[Build][Warning] :: {msg}')

def err(msg):
    print(f'[Build][Warning] :: {msg}')

def panic(msg, exit_code = 1):
    err(msg)
    sys.exit(exit_code)

def run(cmd, shell = True, stdout = None, stderr = None, capture_output = False, text = None):
    return sb.run(cmd, shell=shell, stdout=stdout, stderr=stderr, capture_output=capture_output, text=text)

def get_cmake_presets():
    # Pythonic autism
    res = run('cmake --list-presets', shell=True, stdout=sb.PIPE, stderr=sb.PIPE)
    output = res.stdout.decode('utf-8')
    if output == "":
        return []
    presets = output.replace(' ', '').split('\n')
    presets = presets[2:-1]
    presets = [(s[:s.rfind('"')])[1:] for s in presets] # lol
    return presets

args = sys.argv[1:]
for i in range(0, len(args)):
    arg = args[i]
    larg = arg.lower()
    if larg == 'list':
        log('Listing available presets')
        presets = get_cmake_presets()
        for preset in presets:
            print(preset)
        action = Action.Nope
    elif larg == 'build':
        action = Action.Build
    elif larg.startswith('preset='):
        action = Action.Build
        preset = arg.split('=')[1]
    elif larg == '--no-out':
        stdoutput = sb.PIPE

if action == Action.Build:
    if preset != None:
        log('User defined preset')
        res = run(f'cmake --preset={preset}', stdout=stdoutput, stderr=stdoutput)
        if res.returncode != 0:
            if not preset in get_cmake_presets():
                panic(f'Build failed because \'{preset}\' is not an actual preset.')

            else:
                panic('Build failed for unknown reason(s).')

        else:
            Chrono.begin()
            log('CMake generation started.')
            run(f'cmake --preset={preset}', stdout=stdoutput)
            elapsed = Chrono.end()
            log('CMake generation finished. Took: ' + '{:.2f}'.format(elapsed) + 'ms')
            Chrono.begin()
            run(f'cmake --build builds/{preset}', stdout=stdoutput)
            elapsed = Chrono.end()
            log(f'CMake build finished. Took: ' + '{:.2f}'.format(elapsed) + 'ms')

    else:
        presets = get_cmake_presets()
        if not presets:
            panic(f'Platform not supported.')
            log(f'Defaulting to: {presets[0]}')
