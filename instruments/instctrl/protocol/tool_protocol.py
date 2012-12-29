ourDir = Dir('.').abspath
ourName = 'protocol'

tools = ['doxygen','json', 'evp', 'qt4', 'qtt_qtconfig']
env = Environment(tools = ['default'] + tools)
env.EnableQt4Modules(['QtCore'])

def protocol(env):
    env.AppendUnique(CPPPATH = [ourDir,])
    env.AppendUnique(LIBPATH=[ourDir,])
    env.AppendUnique(LIBS=['protocol',])
    env.AppendDoxref(doxref[0])
    env.Require(tools)
    env.EnableQt4Modules(['QtCore'])

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
