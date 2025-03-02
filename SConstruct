# -*- python -*-

import os
import sys
import SCons
sys.path.append('vardb/site_scons')
import eol_scons

def nimbusbase(env):
    env.Require(['prefixoptions', 'buildmode', 'openmotif', 'nidas', 'netcdf', 'gsl', 'postgres_pq', 'boost_regex', 'bz2', 'z'])
    env.Append(CPPPATH=['#/include', '#/src/nimbus'])
    env.Append(CXXFLAGS=['-std=c++11'])
    env.Append(CXXFLAGS=['-g'])
    env.Append(CXXFLAGS=Split("-Wno-write-strings -Wstrict-aliasing "))
    env['CC'] = env['CXX']
    env['CCFLAGS'] = env['CXXFLAGS']
    env['PUBLISH_PREFIX'] = '/net/www/docs/raf/Software'


env = Environment(platform='posix', tools=['default', 'gitinfo', nimbusbase])
#                 GLOBAL_TOOLS=['scanbuild'])

env.GitInfo('include/gitInfo.h', '.')

Export('env')

##
##  Build include files.
##
SConscript('include/SConscript')

##
##  Build NCAR COS blocked file decoding library.
##
SConscript('src/ncaru/src/SConscript')

##
##  Build ADS2 Header API.
##
SConscript('src/hdr_api/SConscript')

##
##  Build Derived Calculations libraries.
##
SConscript('src/amlib/SConscript')

##
##  Build nimbus proper.
##
SConscript('src/nimbus/SConscript')

SConscript('tests/SConscript')

ninja = Environment(tools = ['default', 'ninja'])
ninja.NinjaCheck()

variables = env.GlobalVariables()
variables.Update(env)

env.SetHelp()

if "publish" in COMMAND_LINE_TARGETS:
   pub = env.Install('$PUBLISH_PREFIX', ["doc/html/netCDF.html", 'doc/html/netCDF_1_3.html', 'doc/html/TimeExamp.html'])
   env.Alias('publish', pub)

env.SetHelp()
env.AddHelp("""
Targets:
publish:  Copy html documentation to EOL web space : $PUBLISH_PREFIX.
""")
