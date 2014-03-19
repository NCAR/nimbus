# -*- python -*-

import os
env = Environment(platform = 'posix',ENV= os.environ)

env.realtime = ARGUMENTS.get('rt', 1)
np = ARGUMENTS.get('NIDAS_PATH', '/opt/nidas')
# Convert paths to absolute, relative to top directory
np = env.Dir("#").Dir(env.subst(np)).get_abspath()
env['NIDAS_PATH'] = np

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
    -Wall -g -Wno-write-strings -Wstrict-aliasing
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

##
##  Install support scripts.
##
SConscript('src/scripts/SConscript')

Alias('install', [env['JLOCAL'] + '/bin'])


##
## Builder to generate nimbus batch files by searching for data files on the system.
##
from SCons.Node import FS
from SCons.Node.Python import Value

def bnimbus_emitter_value(target, source, env):
    # The source is the data file to find, such as 'IDEAS-4-GV/20130926_183907_rf02.ads'
    print("Computing contents for %s..." % (str(target[0])))
    dname = str(source[0])
    dpath = dname
    for ddir in [os.getenv('RAW_DATA_DIR'), '$RAW_DATA_DIR', '$HOME/Data/raf/Raw_Data']:
        if not ddir:
            continue
        dpath = os.path.join(env.subst(ddir), dname)
        if os.path.exists(dpath):
            print("Found %s..." % (dpath))
            source = [File(dpath)]
            break
        else:
            print("Checked but not found: %s..." % (dpath))
    bfile = """
if=%s
of=%s.nc
""" % (source[0].get_abspath(), target[0].path)
    # Don't add the data file as an actual dependency, otherwise SCons
    # consumes too much time calculating checksums.
    return target, [Value(bfile)]

def bnimbus_build_value(env, target, source):
    print("Generating %s..." % (str(target[0])))
    out = open(target[0].path, "w")
    out.write(source[0].get_contents())
    out.write("\n")
    out.close()

nimbus_batch_builder = Builder(
    action = Action(bnimbus_build_value),
    source_factory = FS.default_fs.Entry,
    emitter = bnimbus_emitter_value)

env['BUILDERS']['NimbusBatchFile'] = nimbus_batch_builder


batchfile = env.NimbusBatchFile('batch_rf02', 'IDEAS-4-GV/20130926_183907_rf02.ads')


btest = env.Command('btest', ['src/filter/nimbus', batchfile],
                    '${SOURCES[0]} -b ${SOURCES[1]}')
env.Alias('btest', btest)


env['VALGRIND'] = 'valgrind'
env['VALGRIND_OPTS'] = Split("""
--gen-suppressions=all
--suppressions=valgrind-suppressions.txt
--leak-check=full
--show-possibly-lost=no
""")

vg = env.Command('vg', ['src/filter/nimbus', batchfile],
                 '$VALGRIND $VALGRIND_OPTS ${SOURCES[0]} -b ${SOURCES[1]}')
env.Alias('vg', vg)
