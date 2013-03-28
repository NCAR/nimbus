import os
import commands


# The jsoncpp prefix is the name of the jsoncpp tarfile,
# as well as the directory that it is unpacked into.
jsoncpp_prefix = 'jsoncpp-src-0.6.0-rc2'

# The jsoncpp scons build uses the compiler name
# in the library name, an also places the built library
# in a subdirectory with this name.
gccversion = commands.getoutput('g++ -dumpversion')
gcc_prefix = 'linux-gcc-'+gccversion

# The generatd library name, as passed to the linker
jsoncpplib = "json_"+gcc_prefix+"_libmt"

ourDir = Dir('.').abspath

def build_jsoncpp(env):
    """Unpack and build jsoncpp, if the library is not found"""
    
    libname = 'lib'+jsoncpplib+'.a'
    libdir = ourDir + '/' + jsoncpp_prefix+'/libs/' + gcc_prefix
    print 'looking for ', libname, 'in', libdir
    node = env.FindFile(libname, libdir)
    if node != None:
        return
    
    tarfile = jsoncpp_prefix + ".tar.gz"
    tarcmd = "tar -xvf " + tarfile
    os.system(tarcmd)
    cdcmd = "cd " + jsoncpp_prefix + ";"
    sconscmd = "scons platform=linux-gcc"
    os.system(cdcmd + sconscmd)


# The tool definition
def jsoncpp(env):
    env.Append(LIBS=[jsoncpplib,])
    jsoncppdir = '#/json/'+jsoncpp_prefix
    env.Append(LIBPATH=[jsoncppdir+'/libs/'+gcc_prefix,])
    env.AppendUnique(CPPPATH=[jsoncppdir+'/include',])
    
Export('jsoncpp')

# Unpack and build jsoncpp, as needed
env = Environment()
build_jsoncpp(env)	