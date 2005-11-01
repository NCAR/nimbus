# -*- python -*-

import os
env = Environment(platform = 'posix',ENV= os.environ)

env.realtime = ARGUMENTS.get('rt', 1)

import SCons
file = "sconsign-%s" % (SCons.__version__)
print "Using SConsignFile: %s" % (file)
SConsignFile(File(file).abspath)

# No RCS/SCCS checkout.
env.SourceCode(['.', 'amlib', 'filter'], None)

env['JLOCAL'] = os.environ['JLOCAL']

env['CC'] = ['g++']

env['CPPPATH'] = Split("""
    #/include
    $JLOCAL/include
""")

env['CCFLAGS'] = Split("""
    -Wall -g -O2
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

Alias('install', [env['JLOCAL'] + '/bin'])
