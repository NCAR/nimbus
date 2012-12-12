tools = ['protocol', 'doxygen', 'qt4',]
env = Environment(tools = ['default'] + tools)
qtmods = ['QtCore', 'QtGui', 'QtNetwork',]
env.EnableQt4Modules(qtmods)

sslDir = Dir('.').abspath

def ssl(env):
    env.AppendUnique(CPPPATH = sslDir)
    env.AppendUnique(LIBPATH=[sslDir])
    env.AppendUnique(LIBS=['ssl'])
    env.AppendDoxref(doxref[0])
    env.Require(tools)
    qtmods = ['QtCore', 'QtNetwork',]
    env.EnableQt4Modules(qtmods)

Export('ssl')

sources = Split("""
 SslProxy.cpp
 SslClientConnection.cpp
 SslServerConnection.cpp
 SslServer.cpp
 SslSocket.cpp
""")
headers = Split("""
 SslProxy.h
 SslClientConnection.h
 SslServerConnection.h
 SslServer.h
 SslSocket.h
""")

lib = env.Library('ssl', sources)
Default(lib)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "ssl library" })
doxref = env.Apidocs(sources + headers)

SConscript("test/SConscript")
