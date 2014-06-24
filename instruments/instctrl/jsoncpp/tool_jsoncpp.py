import os
import sys
import subprocess

# Unpack and build jsoncpp. The distribution should be in this directory.
# Jsoncpp is built using scons.

# The jsoncpp prefix is the basename of the jsoncpp tarfile,
# e.g. jsoncpp-src-0.6.0-rc2.tar.gz,
# as well as the directory that it is unpacked into.
json_name = 'jsoncpp-src-0.6.0-rc2'

def runcmd(args, shell=False):
	proc = subprocess.Popen(args, stdout=subprocess.PIPE, shell=shell)
	out, err = proc.communicate()
	out = out.rstrip()
	return out

# The configuration variables specified next are for Linux. OS 
# specializations follow.

# The jsoncpp scons build uses the compiler name
# in the library name, and also places the built library
# in a subdirectory with this name.
gcc_version = runcmd(['g++', '-dumpversion'])
gcc_prefix = 'linux-gcc-'+gcc_version

env = Environment()

# Our directory
this_dir = Dir('.').abspath

# The top level of the unpacked distribution
json_dir = this_dir+'/'+json_name

# The library name, as used by the linker -lswitch
json_libname = "json_"+gcc_prefix+"_libmt"

# The name of library file
json_libfilename = 'lib'+json_libname+'.a'

# The name of dynamic library file
json_dynlibfilename = 'lib'+json_libname+'.so'

# The library directory, as used by the linker -L switch
json_libdir = this_dir + '/' + json_name+'/libs/' + gcc_prefix

# The command to do a scons build
scons_bldcmd   = "cd " + json_name + "; scons    platform=linux-gcc"

# The command to do a scons clean
scons_cleancmd = "cd " + json_name + "; scons -c platform=linux-gcc"

# OSX customization
if sys.platform == 'darwin':
	scons = '/usr/local/bin/scons'
	json_dynlibfilename = 'lib'+json_libname+'.dylib'

# Windows customization
if sys.platform == 'win32':
	scons_bldcmd   = "cd " + json_name + "& scons    platform=mingw"
	scons_cleancmd = "cd " + json_name + "& scons -c platform=mingw"
	json_libname   = "json_mingw_libmt"
	json_libdir    = this_dir + '/' + json_name + '/libs/mingw'	

# Unpack and build jsoncpp
def build_jsoncpp(env):
    """Unpack and build jsoncpp, if the library is not found"""
    
    if (env.GetOption('clean')):
    	if env.FindFile(json_name, '.'):
    		return
		# Clean jsoncpp.
    	print runcmd(scons_cleancmd, shell=True)
    	return

    # See if the library file exists, indicating that the build
    # was already performed.
    lib = env.FindFile(json_libfilename, json_libdir)
    if lib != None:
        return

    # Unpack the distribution tarfile.
    tarfile = json_name + ".tar.gz"
    tarcmd = "tar -xvf " + tarfile
    print runcmd(tarcmd, shell=True)
    
    # Use scons to build jsoncpp.
    cdcmd = "cd " + json_name + "&"
    print runcmd(scons_bldcmd, shell=True)

    # Remove the shared object library, to force static linking.
    print runcmd('rm -f ' + json_libdir+'/'+json_dynlibfilename, shell=True)
 
# Unpack and build or clean jsoncpp
build_jsoncpp(env)	

# The jsoncpp tool definition.
def jsoncpp(env):
    env.Append(LIBS=[json_libname,])
    env.Append(LIBPATH=[json_libdir,])
    env.AppendUnique(CPPPATH=[json_dir+'/include',])
    
Export('jsoncpp')
