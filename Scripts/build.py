#!/usr/bin/env python3

import os
import re
import sys
import time
import shutil
import platform
from enum import Enum
import com

class Action:
    Nope    = 0,
    Build   = 1,
    Clear   = 2,
    Install = 3

action = Action.Build
auxiliary_action = Action.Nope
preset = None
stdoutput = None
parallel = True

args = sys.argv[1:]
for i in range(0, len(args)):
    arg = args[i]
    larg = arg.lower()
    if larg == 'list':
        com.log('Listing available presets')
        presets = com.get_cmake_presets()
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
    cmake_gen = True

    if preset == None:
        possible_builds = None
        common_el = None
        if os.path.isdir("./builds/"):
            possible_builds = os.listdir("./builds/")

        presets = com.get_cmake_presets()
        if not presets:
            com.panic(f'Platform not supported.')

        if possible_builds:
            common_el = set(possible_builds).intersection(presets)

        if common_el:
            preset = list(common_el)[0] # LOL
            cmake_gen = False # Assume the user has already generated the build files.
            com.log(f'Building: {preset}')
        else:
            com.log(f'Defaulting to: {presets[0]}')
            preset = presets[0]

    com.Chrono.begin()
    com.log('CMake generation started.')
    res = com.run(f'cmake --preset={preset}', stdout=stdoutput, stderr=stdoutput)
    if res.returncode != 0:
        if not preset in com.get_cmake_presets():
            com.panic(f'Build failed because \'{preset}\' is not an actual preset.')
        else:
            com.panic('CMake generation failed for unknown reason(s).')

    elapsed = com.Chrono.end()
    com.log('CMake generation finished. Took: ' + '{:.2f}ms'.format(elapsed))

    com.Chrono.begin()
    res = com.run(f'cmake --build builds/{preset}' + (' --parallel' if parallel else ''), stdout=stdoutput)
    if res.returncode != 0:
        com.panic('Build failed for unknown reason(s).')

    elapsed = com.Chrono.end()
    com.log(f'CMake build finished. Took: ' + '{:.2f}ms'.format(elapsed))

    #  Might write a separate script for installation.
    if auxiliary_action == Action.Install:
        com.log('CMake installation started.')
        com.Chrono.begin()

        if platform.system() in ['Linux', 'Darwin']:
            res = com.run(f'sudo cmake --install builds/{preset}', stdout=stdoutput)
        elif platform.system() == 'Windows':
            com.panic("I haven't figured out how to ask for admin privileges on python for windows yet")

        if res.returncode != 0:
            com.panic('CMake install unexpectedly failed.')

        elapsed = com.Chrono.end()
        com.log(f'CMake installation finished. Took: ' + '{:.2f}ms'.format(elapsed))

elif action == Action.Clear:
    if preset == None:
        presets = com.get_cmake_presets()
        if not presets:
            com.panic(f'Platform not supported.')
        com.log(f'Defaulting to: {presets[0]}')
        preset = presets[0]

    com.Chrono.begin()
    com.run(f'cmake --build builds/{preset} --target clean')
    elapsed = com.Chrono.end()
    com.log(f'CMake clean finished. Took: ' + '{:.2f}ms'.format(elapsed))

com.log(f'Done.')
