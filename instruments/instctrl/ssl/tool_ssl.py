tools = ['protocol', 'doxygen', 'qt4', 'ricutil']
env = Environment(tools = ['default'] + tools)
qtmods = ['QtCore', 'QtGui', 'QtNetwork',]
env.EnableQt4Modules(qtmods)

sslDir = Dir('.').abspath

def ssl(env):
    env.AppendUnique(CPPPATH = sslDir)
    env.AppendUnique(LIBPATH=[sslDir])
    env.AppendUnique(LIBS=['ricssl'])
    env.AppendDoxref(doxref[0])
    env.Require(tools)
    qtmods = ['QtCore', 'QtNetwork',]
    env.EnableQt4Modules(qtmods)

Export('ssl')

sources = Split("""
 SslProxy.cpp
 SslConnection.cpp
 SslServer.cpp
 SslSocket.cpp
""")

headers = Split("""
 SslProxy.h
 SslConnection.h
 SslServer.h
 SslSocket.h
""")

lib = env.Library('ricssl', sources)
Default(lib)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "RIC ssl library" })
doxref = env.Apidocs(sources + headers)

SConscript("test/SConscript")
