#!/usr/bin/env python

# This is a helper script to re-link all of the .dylib files,
# copy them into the .app bundle, and change libpng linking
# so that it links against the homebrew libpng (since the XQuartz
# version has a bad load command in it)

import os
import shutil
import subprocess

def get_libs(filename):
    otool = subprocess.check_output('otool -L %s' % filename, shell=True)
    L = otool.split('\n')[1:-1]
    L = map(lambda d: d[1:].split(' ')[0], L)
    L = filter(lambda d: '/usr/lib' not in d and
                         '/System' not in d and
                         '@executable_path' not in d, L)
    return set(L)

def relink(filename):
    libs = get_libs(filename)
    for L in libs:
        subprocess.call(
            'install_name_tool -change %s @executable_path/../libs/%s %s' %
            (L, os.path.basename(L), filename),
            shell=True)

old = None
libs = get_libs('carvr.app/Contents/MacOS/carvr')
while old != libs:
    old = libs
    for L in libs:  libs = libs.union(get_libs(L))

# special case to work around bad XQuartz libpng
libs = map(lambda d: '/usr/local/Cellar/libpng/1.5.14/lib/libpng15.15.dylib'
                  if 'libpng' in d else d, libs)

# Create a 'libs' subfolder
d = 'carvr.app/Contents/libs/'
if os.path.exists(d):   shutil.rmtree(d)
os.makedirs(d)

# And copy over all of the libraries
for L in libs:
    name = os.path.basename(L)
    new_loc = os.path.join(d, name)
    new_id = '@executable_path/../libs/%s' % name
    shutil.copyfile(L, new_loc)
    subprocess.call(
        'install_name_tool -id %s %s' % (new_id, new_loc), shell=True)
    subprocess.call(
        'chmod +w %s' % new_loc, shell=True)
    relink(new_loc)

relink('carvr.app/Contents/MacOS/carvr')





