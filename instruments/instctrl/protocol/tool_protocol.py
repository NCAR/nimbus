import sys

ourDir = Dir('.').abspath
ourName = 'protocol'

tools = ['doxygen','jsoncpp', 'evp', 'qt4', 'qtt_qtconfig']
env = Environment(tools = ['default'] + tools)
env.EnableQt4Modules(['QtCore'])

def protocol(env):
    env.AppendUnique(CPPPATH = [ourDir,])
    env.AppendUnique(LIBPATH=[ourDir,])
    env.AppendUnique(LIBS=['protocol',])
    env.AppendDoxref(doxref[0])
    env.Require(tools)
    env.EnableQt4Modules(['QtCore'])
    if (sys.platform=='win32'):
        env.Append(LIBS=['gdi32'])
    

Export(ourName)

sources = Split("""
 InstConfig.cpp
 StreamMsgProtocol.cpp
 SymCipherProtocol.cpp
 Message.cpp
 MessagePayload.cpp
 Protocol.cpp
""")

headers = Split("""
 InstConfig.h
 StreamMsgProtocol.h
 SymCipherProtocol.h
 Message.h
 MessagePayload.h
 Protocol.h
""")

lib = env.Library(ourName, sources)
Default(lib)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : ourName + " library" })
doxref = env.Apidocs(sources + headers)

SConscript("test/SConscript")
