# libbone - A library for working with the BeagleBone in C
# Copyright (C) 2012 Clayton G. Hobbs

# Import some stuff.
import sys, os
import SCons.Util

Import('package_version') # You know, that thing we did in SConstruct.


opts = Variables('custom.py', ARGUMENTS)
opts.Add('BUILD', 'Set the build type', 'default')
opts.Add('CC', 'C Compiler', 'gcc')

opts.Add('CPPPATH',    'Additional preprocessor paths', [])
opts.Add('LIBPATH',    'Additional library paths',      [])
opts.Add('CPPFLAGS',   'Additional preprocessor flags', [])
opts.Add('CPPDEFINES', 'defined constants', [])
opts.Add('LIBS',       'Additional libraries', [])
opts.Add('CCFLAGS',    'C Compiler flags', ['-g', '-std=gnu99', '-Wall'])
opts.Add('LINKFLAGS',  'Linker Compiler flags', ['-Wl,-soname,libbone.so.0'])

env = Environment(options=opts)
env.Append(CPPDEFINES = [('VERSION', '"\\"%s\\""' % package_version)])

Help(opts.GenerateHelpText(env))

conf = env.Configure()

# Let's keep doing the configuring thing.

#conf.env.ParseConfig('pkg-config --cflags --libs example_library')

env = conf.Finish()

# All done configuring now; time to build.

libbone = env.SharedLibrary('libbone', Glob('src/*.c'),
                             SHLIBSUFFIX='.so.' + package_version)
env.Default(libbone)

# That should be it, actually.
