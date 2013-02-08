import sys

ourDir = Dir('.').abspath
ourName = 'ricutil'

tools = ['doxygen', 'qt4', 'boost', 'boost_date_time']
env = Environment(tools = ['default'] + tools)
env.EnableQt4Modules(['QtCore', 'QtNetwork'])

def ricutil(env):
    env.AppendUnique(CPPPATH = [ourDir,])
    env.AppendUnique(LIBPATH=[ourDir,])
    # The zlib library
    env.Append(LIBS=[ourName,'z'])
    # The bzip2 library
    env.Append(LIBS=[ourName,'bz2'])    
    #env.AppendDoxref(doxref[0])
    env.Require(tools)
    env.EnableQt4Modules(['QtCore'])
    if (sys.platform=='win32'):
        env.Append(LIBS=['gdi32'])
    

Export(ourName)

sources = Split("""
 BzipCompress.cpp
 QtAddress.cpp
 RicLogger.cpp
 ZlibCompress.cpp
""")

headers = Split("""
 BzipCompress.h
 QtAddress.h
 RicLogger.h
 ZlibCompress.h
""")

lib = env.Library(ourName, sources)
Default(lib)

#env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : ourName + " library" })
#doxref = env.Apidocs(sources + headers)
