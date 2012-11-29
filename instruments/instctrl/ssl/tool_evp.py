tools = ['doxygen',]
env = Environment(tools = ['default'] + tools)

evpDir = Dir('.').abspath

def evp(env):
    env.AppendUnique(CPPPATH = evpDir)
    env.AppendUnique(LIBPATH=[evpDir])
    env.AppendUnique(LIBS=['evp'])
    env.AppendUnique(LIBS=['crypto'])
#    env.AppendDoxref(doxref[0])
    env.Require(tools)

Export('evp')

sources = Split("""
 EVPCipher.cpp
""")

headers = Split("""
 EVPCipher.h
""")

lib = env.Library('evp', sources)
Default(lib)

#env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "evp library" })
#doxref = env.Apidocs(sources + headers)
