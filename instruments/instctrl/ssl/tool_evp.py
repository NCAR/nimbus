import sys
tools = ['doxygen']
env = Environment(tools = ['default'] + tools)

evpDir = Dir('.').abspath

def evp(env):
    env.AppendUnique(CPPPATH = evpDir)
    env.AppendUnique(LIBPATH=[evpDir])
    env.AppendUnique(LIBS=['evp'])
    env.AppendUnique(LIBS=['crypto'])
    if (sys.platform=='win32'):
        env.Append(LIBS=['gdi32'])
#    env.AppendDoxref(doxref[0])
    env.Require(tools)

Export('evp')

sources = Split("""
 EVPCipher.cpp
 base64.cpp
""")

headers = Split("""
 EVPCipher.h
 base64.h
""")

lib = env.Library('evp', sources)
Default(lib)

#env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "evp library" })
#doxref = env.Apidocs(sources + headers)
