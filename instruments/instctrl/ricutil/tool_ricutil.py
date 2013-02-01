import sys

ourDir = Dir('.').abspath
ourName = 'ricutil'

tools = ['doxygen', 'qt4',]
env = Environment(tools = ['default'] + tools)
env.EnableQt4Modules(['QtCore', 'QtNetwork'])

def ricutil(env):
    env.AppendUnique(CPPPATH = [ourDir,])
    env.AppendUnique(LIBPATH=[ourDir,])
    env.AppendUnique(LIBS=[ourName,'z'])
    #env.AppendDoxref(doxref[0])
    env.Require(tools)
    env.EnableQt4Modules(['QtCore'])
    if (sys.platform=='win32'):
        env.Append(LIBS=['gdi32'])
    

Export(ourName)

sources = Split("""
 QtAddress.cpp
 RicLogger.cpp
 ZlibCompress.cpp
""")

headers = Split("""
 QtAddress.h
 RicLogger.h
 ZlibCompress.h
""")

lib = env.Library(ourName, sources)
Default(lib)

#env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : ourName + " library" })
#doxref = env.Apidocs(sources + headers)
