ourDir = Dir('.').abspath
ourName = 'protocol'

tools = ['doxygen','json']
env = Environment(tools = ['default'] + tools)

def protocol(env):
    env.AppendUnique(CPPPATH = [ourDir,])
    env.AppendUnique(LIBPATH=[ourDir,])
    env.AppendUnique(LIBS=['protocol',])
    env.AppendDoxref(doxref[0])
    env.Require(tools)

Export(ourName)

sources = Split("""
 Message.cpp
 MessagePayload.cpp
""")

headers = Split("""
 Message.h
 MessagePayload.h
""")

lib = env.Library(ourName, sources)
Default(lib)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : ourName + " library" })
doxref = env.Apidocs(sources + headers)

SConscript("test/SConscript")