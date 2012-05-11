# libbone - A library for working with the BeagleBone in C
# Copyright (C) 2012 Clayton G. Hobbs

import os # I want to open files


with open('VERSION', 'r') as ver:
    package_version = ver.readline().strip()

Export('package_version')
SConscript('SConscript', variant_dir='build', duplicate=0)
