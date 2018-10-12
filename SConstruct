# -*- python -*-

import os
import sys

# Rather than depend upon finding eol_scons somewhere else in the RAF
# source tree, expect it to be on the standard search path:
# /usr/share/scons/site_scons or ~/.scons/site_scons.
import eol_scons

def nimbusbase(env):
    env.Require(['buildmode', 'nidas', 'netcdf', 'jlocal'])
    env['CC'] = env['CXX']
    env.Append(LIBPATH=['$JLOCAL/lib'])
    env.Append(CPPPATH=['#/include', '#/src/filter'])
    env.Append(CCFLAGS=Split("-Wno-write-strings -Wstrict-aliasing "
                             "-Wno-deprecated"))

env = Environment(platform='posix', tools=['default', 'svninfo', nimbusbase])
#                 GLOBAL_TOOLS=['scanbuild'])

env.SvnInfo('include/svnInfo.h', '.')

env['CXXFLAGS'] = Split("""
    -std=c++11
""")

Export('env')

##
##  Build include files.
##
SConscript('include/SConscript')

##
##  Build Derived Calculations libraries.
##
SConscript('src/ncaru/src/SConscript')

##
##  Build Derived Calculations libraries.
##
SConscript('src/amlib/SConscript')

##
##  Build nimbus proper.
##
SConscript('src/filter/SConscript')

Alias('install', env.subst("${JLOCAL}/bin"))

SConscript('tests/SConscript')

ninja = Environment(tools = ['default', 'ninja'])
ninja.NinjaCheck()

variables = env.GlobalVariables()
variables.Update(env)

env.SetHelp()
