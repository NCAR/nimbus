
# The default jsoncpp library is /usr/local/lib/libjsoncpp.a.
# Set JSONCPP_LIB to specify a different library.
#
# The default jsoncpp header location is /usr/local/include.
# Set JSONCPP_INCLUDE to specify a different include location. Note
# that the header files will be in a json/ subdirectory.

import os

def jsoncpp(env):
    options = env.GlobalVariables()
    options.AddVariables(PathVariable('JSONCPP_LIB', 'Path to the jsoncpp library, e.g. /usr/local/lib/libjsoncpp_mingw.a.', None))
    options.AddVariables(PathVariable('JSONCPP_INCLUDE', 'Path to the jsoncpp header directory, e.g. /usr/local/include', None))
    options.Update(env)
    
    if env.has_key('JSONCPP_LIB'):
        jsoncpp_lib = env['JSONCPP_LIB']
    else:
         jsoncpp_lib = '/usr/local/lib/libjsoncpp.a'

    if env.has_key('JSONCPP_INCLUDE'):
        jsoncpp_include = env['JSONCPP_INCLUDE']
    else:
         jsoncpp_include = '/usr/local/include'

    jsoncpp_libdir = os.path.dirname(jsoncpp_lib)

    jsoncpp_libname = jsoncpp_lib
    jsoncpp_libname = jsoncpp_libname.replace(jsoncpp_libdir + '/', '')
    
    env.AppendUnique(CPPPATH = [jsoncpp_include,])
    env.AppendUnique(LIBPATH=[jsoncpp_libdir,])
    env.AppendUnique(LIBS=[jsoncpp_libname,])
    
Export('jsoncpp')
	