# -*- python -*-

import os
env = Environment(platform = 'posix',ENV= os.environ)

env.realtime = ARGUMENTS.get('rt', 1)
np = ARGUMENTS.get('NIDAS_PATH', '/opt/local/nidas')
# Convert paths to absolute, relative to top directory
np = env.Dir("#").Dir(env.subst(np)).get_abspath()
env['NIDAS_PATH'] = os.path.join(np, 'x86')

import SCons
file = "sconsign-%s" % (SCons.__version__)
print "Using SConsignFile: %s" % (file)
SConsignFile(File(file).abspath)

# No RCS/SCCS checkout.
env.SourceCode(['.', 'amlib', 'filter'], None)

# This is mostly for buildbot, which does not use the Jeffco login.
try: env['JLOCAL'] = os.environ['JLOCAL']
except KeyError:
    print "\n$JLOCAL not found!  Defaulting to '/opt/local'\n"
    env['JLOCAL'] = '/opt/local/'

env['CC'] = ['g++']

env['CPPPATH'] = Split("""
    #/include
    $JLOCAL/include
    /usr/include/netcdf
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
