#!/usr/bin/env python3

import os
import re
import sys
import time
import shutil
import platform
from enum import Enum
import subprocess as sb

class Action:
    Nope    = 0,
    Build   = 1,
    Clear   = 2,
    Install = 3

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
auxiliary_action = Action.Nope
preset = None
stdoutput = None
parallel = True

def log(msg):
    print(f'[Build] [Info] :: {msg}')

def warn(msg):
    print(f'[Build] [Warning] :: {msg}')

def err(msg):
    print(f'[Build] [Warning] :: {msg}')

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
        stdoutput = sb.DEVNULL
    elif larg == '--no-parallel':
        parallel = False
    elif larg == '--clear':
        action = Action.Clear
    elif larg == '--install':
        auxiliary_action = Action.Install

if action == Action.Build:
    if preset == None:
        presets = get_cmake_presets()
        if not presets:
            panic(f'Platform not supported.')
        log(f'Defaulting to: {presets[0]}')
        preset = presets[0]

    res = run(f'cmake --preset={preset}', stdout=stdoutput, stderr=stdoutput)
    if res.returncode != 0:
        if not preset in get_cmake_presets():
            panic(f'Build failed because \'{preset}\' is not an actual preset.')

        else:
            panic('Build failed for unknown reason(s).')

    else:
        Chrono.begin()
        log('CMake generation started.')
        res = run(f'cmake --preset={preset}', stdout=stdoutput)
        if res.returncode != 0:
            panic('CMake generation unexpectedly failed.')

        elapsed = Chrono.end()
        log('CMake generation finished. Took: ' + '{:.2f}ms'.format(elapsed))
        Chrono.begin()
        res = run(f'cmake --build builds/{preset}' + (' --parallel' if parallel else ''), stdout=stdoutput)
        if res.returncode != 0:
            panic('CMake build unexpectedly failed.')

        elapsed = Chrono.end()
        log(f'CMake build finished. Took: ' + '{:.2f}ms'.format(elapsed))

        #  Might write a separate script for installation.
        if auxiliary_action == Action.Install:
            log('CMake installation started.')
            Chrono.begin()

            if platform.system() in ['Linux', 'Darwin']:
                res = run(f'sudo cmake --install builds/{preset}', stdout=stdoutput)
            elif platform.system() == 'Windows':
                panic("I haven't figured out how to ask for admin privileges on python for windows yet")

            if res.returncode != 0:
                panic('CMake install unexpectedly failed.')

            elapsed = Chrono.end()
            log(f'CMake installation finished. Took: ' + '{:.2f}ms'.format(elapsed))

elif action == Action.Clear:
    if preset == None:
        presets = get_cmake_presets()
        if not presets:
            panic(f'Platform not supported.')
        log(f'Defaulting to: {presets[0]}')
        preset = presets[0]

    Chrono.begin()
    run(f'cmake --build builds/{preset} --target clean')
    elapsed = Chrono.end()
    log(f'CMake clean finished. Took: ' + '{:.2f}ms'.format(elapsed))

log(f'Done.')
